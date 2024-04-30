/**
 ******************************************************************************
 * @file           : resourceDownloader.cpp
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 24-4-28
 ******************************************************************************
 */

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <utility>
#include "resourceDownloader.h"
#include "logger.h"

ResourceDownloader::ResourceDownloader(QObject *parent) :
    QObject(parent){
    m_ossClient = nullptr;
    m_cloudFiles = new std::vector<std::string>;
    m_ossInfo = new std::map<std::string, std::string>;

    init();
}

void ResourceDownloader::init() {
    AlibabaCloud::OSS::ClientConfiguration conf;
    AlibabaCloud::OSS::InitializeSdk();

    initOssInfo();
    m_ossClient = new AlibabaCloud::OSS::OssClient(m_ossInfo->at(m_endPoint), m_ossInfo->at(m_accessKeyId),
                                                   m_ossInfo->at(m_accessKeySecret), conf);

    /* 列举文件。*/
    std::string nextMarker;
    bool isTruncated = false;
    do {
        /* 列举文件。*/
        AlibabaCloud::OSS::ListObjectsRequest request(m_ossInfo->at(m_bucketName));
        request.setMarker(nextMarker);
        auto outcome = m_ossClient->ListObjects(request);

        if (!outcome.isSuccess()) {
            /* 异常处理。*/
            std::stringstream info;
            info << "ListObjects fail" << ",code:" << outcome.error().Code() << ",message:" << outcome.error().Message() << ",requestId:" << outcome.error().RequestId();
            Logger::getInstance()->log(Logger::Error, QString(__FUNCTION__) + " "
                                                          + QString::number(__LINE__) + " "
                                                          + info.str().c_str());
            AlibabaCloud::OSS::ShutdownSdk();
            return;
        } else {
            for (const auto &object : outcome.result().ObjectSummarys()) {
                if (!object.Key().ends_with("/")) {
                    m_cloudFiles->emplace_back(object.Key());
                }
            }
        }
        nextMarker = outcome.result().NextMarker();
        isTruncated = outcome.result().IsTruncated();
    } while (isTruncated);
}

void ResourceDownloader::initOssInfo() {
    QFile accessKeyJsonFile(":/json/OssInfo.json");
    if (!accessKeyJsonFile.exists() || !accessKeyJsonFile.open(QIODevice::OpenModeFlag::ReadOnly)) {
        accessKeyJsonFile.close();
        Logger::getInstance()->log(Logger::Error, QString(__FUNCTION__) + " "
                                                      + QString::number(__LINE__));
        return;
    }

    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(accessKeyJsonFile.readAll(), &parseError);
    if (parseError.error != QJsonParseError::ParseError::NoError
        || jsonDocument.isEmpty() || jsonDocument.isNull()) {
        Logger::getInstance()->log(Logger::Error, QString(__FUNCTION__) + " "
                                                      + QString::number(__LINE__));
        return;
    }

    QJsonObject jsonObject = jsonDocument.object();
    (*m_ossInfo)[m_accessKeyId] = jsonObject.take(m_accessKeyId.c_str()).toString().toStdString();
    (*m_ossInfo)[m_accessKeySecret] = jsonObject.take(m_accessKeySecret.c_str()).toString().toStdString();
    (*m_ossInfo)[m_bucketName] = jsonObject.take(m_bucketName.c_str()).toString().toStdString();
    (*m_ossInfo)[m_endPoint] = jsonObject.take(m_endPoint.c_str()).toString().toStdString();
}

ResourceDownloader::~ResourceDownloader() {
    /* 释放网络等资源。*/
    delete m_ossClient;
    delete m_ossInfo;
    AlibabaCloud::OSS::ShutdownSdk();
}

int ResourceDownloader::getFilesSize() const {
    if (m_cloudFiles != nullptr) {
        return (int)m_cloudFiles->size();
    }
    return 0;
}

std::string ResourceDownloader::getFileName(const int &index) const {
    return m_cloudFiles->at(index);
}

void ResourceDownloader::downloadFile(const std::string &fileName, std::function<void(size_t, int64_t, int64_t, void *)> progressCallback) {
    std::cout << __FUNCTION__  << std::endl;
    emit sigDownloadingFile(fileName);
    /* 断点续传下载。*/
    // DownloadObjectRequest request(BucketName, ObjectName, DownloadFilePath, CheckpointFilePath);
    AlibabaCloud::OSS::DownloadObjectRequest request(m_ossInfo->at(m_bucketName), fileName, fileName,
                                                        std::filesystem::path(fileName).parent_path().string());
    
//    std::function<void(size_t, int64_t, int64_t, void *)> progressCallback = std::bind(&ResourceDownloader::progressCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    AlibabaCloud::OSS::TransferProgress transferProgress = {progressCallback, nullptr};
    request.setTransferProgress(transferProgress);
    
    auto outcome = m_ossClient->ResumableDownloadObject(request);
    if (!outcome.isSuccess()) {
        /* 异常处理。*/
        std::stringstream info;
        info << "ResumableDownloadObject fail" << ",code:" << outcome.error().Code() << ",message:" << outcome.error().Message() << ",requestId:" << outcome.error().RequestId();
        Logger::getInstance()->log(Logger::Error, QString(__FUNCTION__) + " "
                                                  + QString::number(__LINE__) + " "
                                                  + info.str().c_str());
        return;
    } else {
        emit sigFileDownloadFinished();
    }
}

void ResourceDownloader::progressCallback(size_t increment, int64_t transfered, int64_t total, void *userData) {
    // increment表示本次回调接收的数据大小。
    // transferred表示已接收的数据大小。
    // total表示下载文件的总大小。
    int value = (int)((transfered * 100) / total);
    emit sigUpdateProgressBar(value);
    std::cout << __FUNCTION__ << "transferred:" << transfered << " total:" << total << " value:" << value << std::endl;
}
