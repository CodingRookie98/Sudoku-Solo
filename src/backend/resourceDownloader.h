/**
 ******************************************************************************
 * @file           : resourceDownloader.h
 * @author         : CodingRookie
 * @brief          : None
 * @attention      : None
 * @date           : 24-4-28
 ******************************************************************************
 */

#ifndef SUDOKU_SOLO_SRC_BACKEND_RESOURCEDOWNLOADER_H_
#define SUDOKU_SOLO_SRC_BACKEND_RESOURCEDOWNLOADER_H_

#include <vector>
#include <alibabacloud/oss/OssClient.h>

class ResourceDownloader : public QObject {
    Q_OBJECT
public:
    ResourceDownloader(QObject *parent = nullptr);
    ~ResourceDownloader() override;

    [[nodiscard]] int getFilesSize() const;
    [[nodiscard]] std::string getFileName(const int &index) const;
    void downloadFile(const std::string &fileName,
                      std::function<void(size_t, int64_t, int64_t, void *)> progressCallback);

signals:
    void sigUpdateProgressBar(const int &value);
    void sigFileDownloadFinished();
    void sigDownloadingFile(const std::string &fileName);

private:
    AlibabaCloud::OSS::OssClient *m_ossClient;
    const std::string m_endPoint = "EndPoint";
    const std::string m_bucketName = "BucketName";
    const std::string m_accessKeyId = "AccessKeyId";
    const std::string m_accessKeySecret = "AccessKeySecret";
    std::vector<std::string> *m_cloudFiles;
    std::map<std::string, std::string> *m_ossInfo;

    void init();
    void initOssInfo();
    void progressCallback(size_t increment, int64_t transfered, int64_t total, void *userData);
};

#endif // SUDOKU_SOLO_SRC_BACKEND_RESOURCEDOWNLOADER_H_
