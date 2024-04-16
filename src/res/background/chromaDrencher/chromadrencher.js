function t(t, e) {
    this.viewPort = { x: 0, y: 0, width: t, height: e }, this.projectionMatrix = null, this.viewMatrix = new Float32Array([1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1])
}

function e(t, e, i, o, r, n) {
    const a = e - t, s = o - i, l = n - r;
    return new Float32Array([2 / a, 0, 0, 0, 0, 2 / s, 0, 0, 0, 0, -2 / l, 0, -(t + e) / a, -(o + i) / s, -(n + r) / l, 1])
}

function i(t, e, i, o, r, n) {
    const a = e - t, s = o - i, l = n - r;
    return new Float32Array([2 * r / a, 0, 0, 0, 0, 2 * r / s, 0, 0, (e + t) / a, (o + i) / s, -(n + r) / l, -1, 0, 0, -n * r * 2 / l, 0])
}

function o(t, e, o, r) {
    const n = o * Math.tan(t * Math.PI / 360), a = n * e;
    return i(-a, a, -n, n, o, r)
}

function r(t, e, i, o) {
    const n = t.createProgram();
    null != o && t.transformFeedbackVaryings(n, o, t.INTERLEAVED_ATTRIBS);
    const a = t.createShader(t.VERTEX_SHADER), s = t.createShader(t.FRAGMENT_SHADER);
    t.attachShader(n, a), t.attachShader(n, s), t.shaderSource(a, e), t.shaderSource(s, i), this.gl = t, this.uniform = {}, this.attribute = {}, this.program = n, this.vertexShader = a, this.fragmentShader = s, this.linked = !1, r.programs.push(this)
}

function n(t, e, i) {
    this.gl = t, this.buffer = t.createBuffer(), this.format = i, null != e && (t.bindBuffer(t.ARRAY_BUFFER, this.buffer), t.bufferData(t.ARRAY_BUFFER, e, t.STATIC_DRAW)), this.stride = 0;
    for (const t of i) t.bytes = t.size * Float32Array.BYTES_PER_ELEMENT, t.offset = this.stride, this.stride += t.bytes
}

function a(t, e, i, o) {
    const r = t.createVertexArray();
    t.bindVertexArray(r);
    for (const o of i) {
        t.bindBuffer(t.ARRAY_BUFFER, o.buffer);
        for (const i of o.format) e.attribute[i.name] && (t.vertexAttribPointer(e.attribute[i.name].location, i.size, t.FLOAT, !1, o.stride, i.offset), t.enableVertexAttribArray(e.attribute[i.name].location))
    }
    if (t.bindBuffer(t.ARRAY_BUFFER, null), null != o) for (const i of o) e.attribute[i.name] && t.vertexAttribDivisor(e.attribute[i.name].location, i.size);
    return t.bindVertexArray(null), r
}

function s(t, e) {
    return t = Math.ceil(t), e = Math.floor(e), Math.floor(Math.random() * (e - t + 1)) + t
}

function l(t) {
    this.front = t(), this.back = t()
}

function h(t, e) {
    const i = e;
    e = t, t = i
}

r.programs = [], r.current = null, r.assemblePrograms = function () {
    const t = r.programs;
    for (const e of t) {
        const t = e.gl;
        t.compileShader(e.vertexShader), t.compileShader(e.fragmentShader)
    }
    for (const e of t) {
        e.gl.linkProgram(e.program)
    }
    for (const e of t) {
        const t = e.gl;
        if (!t.getProgramParameter(e.program, t.LINK_STATUS)) throw console.error("Link fail: " + t.getProgramInfoLog(e.program)), console.error("Vertex: " + t.getShaderInfoLog(e.vertexShader)), console.error("Fragment: " + t.getShaderInfoLog(e.fragmentShader)), "Shader failure";
        e.linked = !0
    }
    for (const e of t) {
        const t = e.gl, i = t.getProgramParameter(e.program, t.ACTIVE_ATTRIBUTES);
        for (let o = 0; o < i; ++o) {
            const i = t.getActiveAttrib(e.program, o), r = t.getAttribLocation(e.program, i.name);
            if (r >= 0) {
                const t = {};
                t.location = r, t.type = i.type, t.size = i.size, e.attribute[i.name] = t
            }
        }
        const o = t.getProgramParameter(e.program, t.ACTIVE_UNIFORMS);
        for (let i = 0; i < o; ++i) {
            const o = t.getActiveUniform(e.program, i), r = o.name.split("[")[0], n = t.getUniformLocation(e.program, o.name);
            if (null != n) {
                const t = {};
                t.location = n, t.type = o.type, t.size = o.size, t.isArray = o.size > 1, e.uniform[r] = t
            }
        }
    }
}, r.prototype.activate = function () {
    r.current != this && (m.useProgram(this.program), r.current = this)
}, r.prototype.setUniform = function (t, e, i, o, r) {
    this.activate();
    const n = this.gl, a = this.uniform[t];
    switch (a.value1 = e, a.value2 = i, a.value3 = o, a.value4 = r, a.type) {
        case n.FLOAT:
            a.isArray ? n.uniform1fv(a.location, e) : n.uniform1f(a.location, e);
            break;
        case n.FLOAT_VEC2:
            a.isArray ? n.unfiorm2fv(a.location, e) : n.uniform2f(a.location, e, i);
            break;
        case n.FLOAT_VEC3:
            a.isArray ? n.uniform3fv(a.location, e) : n.uniform3f(a.location, e, i, o);
            break;
        case n.FLOAT_VEC4:
            a.isArray ? n.uniform4fv(a.location, e) : n.uniform4f(a.location, e, i, o, r);
            break;
        case n.FLOAT_MAT2:
            n.uniformMatrix2fv(a.location, !1, e);
            break;
        case n.FLOAT_MAT3:
            n.uniformMatrix3fv(a.location, !1, e);
            break;
        case n.FLOAT_MAT4:
            n.uniformMatrix4fv(a.location, !1, e);
            break;
        default:
            throw "Shader uniform type not implemented"
    }
}, r.clearAll = function () {
    r.programs = []
}, n.prototype.sizeData = function (t) {
    const e = this.gl;
    e.bindBuffer(e.ARRAY_BUFFER, this.buffer), e.bufferData(e.ARRAY_BUFFER, t.byteLength, e.STREAM_DRAW), e.bindBuffer(e.ARRAY_BUFFER, null)
}, n.prototype.setData = function (t, e, i) {
    const o = this.gl;
    e = e || 0, i = i || t.length, o.bindBuffer(o.ARRAY_BUFFER, this.buffer), o.bufferData(o.ARRAY_BUFFER, t.byteLength, o.STREAM_DRAW), o.bufferSubData(o.ARRAY_BUFFER, e, t, 0, i), o.bindBuffer(o.ARRAY_BUFFER, null)
}, n.prototype.streamData = function (t) {
    const e = this.gl;
    e.bindBuffer(e.ARRAY_BUFFER, this.buffer), e.getBufferParameter(e.ARRAY_BUFFER, e.BUFFER_SIZE) != t.byteLength && (e.bufferData(e.ARRAY_BUFFER, t.byteLength, e.STREAM_DRAW), console.log("Array Buffer resize: " + t.byteLength + " bytes")), e.bufferSubData(e.ARRAY_BUFFER, 0, t, 0, t.length), e.bindBuffer(e.ARRAY_BUFFER, null)
}, n.prototype.getData = function (t) {
    const e = this.gl, i = e.createBuffer();
    e.bindBuffer(e.ARRAY_BUFFER, i), e.bufferData(e.ARRAY_BUFFER, t.byteLength, e.STREAM_READ), e.bindBuffer(e.ARRAY_BUFFER, null), e.bindBuffer(e.COPY_READ_BUFFER, this.buffer), e.bindBuffer(e.COPY_WRITE_BUFFER, i), e.copyBufferSubData(e.COPY_READ_BUFFER, e.COPY_WRITE_BUFFER, 0, 0, t.byteLength), e.bindBuffer(e.COPY_READ_BUFFER, null), e.bindBuffer(e.COPY_WRITE_BUFFER, null), e.finish(), e.bindBuffer(e.ARRAY_BUFFER, i), e.getBufferSubData(e.ARRAY_BUFFER, 0, t), e.bindBuffer(e.ARRAY_BUFFER, null), e.deleteBuffer(i)
}, Array.prototype.shuffle = function () {
    let t, e, i = this.length;
    if (0 == i) return this;
    for (; --i;) t = Math.floor(Math.random() * (i + 1)), e = this[i], this[i] = this[t], this[t] = e;
    return this
}, l.prototype.swap = function () {
    const t = this.back;
    this.back = this.front, this.front = t
};
"use strict";

function c() {
    this.linedensity = 100, this.lineheight = 1024, this.linewidth = 2, this.tipheight = 64, this.tipratio = .1, this.linespeed = 10, this.colorspeed = 30, this.colorprogram = 1, this.linecount = 0, this.ctime = null, this.cup = null, this.linelist = [], this.rotation = 0, this.gradientLength1 = 0, this.gradientLength2 = 0, this.gradientColors1 = [], this.gradientColors2 = [], this.backgroundColor = [0, 0, 0], this.colorList = [[0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0]], this.colorMax = 5, this.colorCustom = [[0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0]], this.preview = !1, this.previewState = {}, this.loadedCount = 0, this.setupChroma()
}

function f(t, e) {
    this.chromad = t, this.gl = e, this.shaderProgram = null, this.particleShaderProgram = null, this.lineBuffer = null, this.linesInUse = 0
}

let d, u, m;

function g() {
    d = document.createElement("div"), d.style.width = "100%", d.style.height = "100%", d.id = "chromadrencher", u = document.createElement("canvas"), u.style.width = "100%", u.style.height = "100%", d.appendChild(u);
    const t = document.getElementsByTagName("script"), e = t[t.length - 1];
    e.parentNode.insertBefore(d, e);
    const i = y.dprscale && window.devicePixelRatio || 1, o = u.getBoundingClientRect();
    u.width = o.width * i, u.height = o.height * i, u.addEventListener("webglcontextcreationerror", b, !1), u.addEventListener("webglcontextlost", F, !1), u.addEventListener("webglcontextrestored", x, !1), console.log("Chroma Drencher v1.2"), p = new c, w(u), L()
}

c.prototype.coolColors = function () {
    let t = this.colorList.slice();
    return t.shuffle(), t = t.slice(0, s(Math.min(this.colorMax, 3), Math.min(this.colorMax, 5))), t
}, c.prototype.colorSpan = function (t) {
    const e = this.coolColors();
    t ? (this.gradientColors1 = [].concat(...e), this.gradientLength1 = e.length) : (this.gradientColors2 = [].concat(...e), this.gradientLength2 = e.length)
}, c.prototype.prepareLines = function () {
    this.linelist = [], this.linecount = Math.floor(this.linedensity * u.width / 100);
    for (let t = 0; t < this.linecount; t++) {
        const t = this.createLine();
        this.linelist.push(t)
    }
}, c.prototype.createLine = function () {
    const t = {};
    t.x = s(0, u.width), t.y = s(0, u.height + this.lineheight), t.sp = 5 * Math.random() + 3, t.tip = !1, t.alpha = s(25, 100) / 255;
    let e = this.tipratio;
    return this.linecount > 4e3 && (e = this.tipratio / 2), this.linecount > 8e3 && (e = this.tipratio / 4), Math.random() < e && (t.tip = !0, t.alpha = 220 / 255), t
}, c.prototype.setupChroma = function () {
    this.prepareLines(), this.applyColors()
}, c.prototype.update = function (t) {
    this.preview ? this.updatePreview(t) : (this.ctime = this.ctime + this.cup * this.colorspeed * t, this.ctime > 255 ? (this.ctime = 255, this.cup = -1, this.colorSpan(!0)) : this.ctime < 0 && (this.ctime = 0, this.cup = 1, this.colorSpan(!1)))
}, c.prototype.updateLines = function (t) {
    u.width;
    const e = u.height;
    for (let i = 0; i < this.linecount; i++) {
        const o = this.linelist[i];
        o.y += o.sp * this.linespeed * t, o.y > e && (o.y = 0 - this.lineheight)
    }
}, c.prototype.setLineCount = function (t) {
    if (t != this.linedensity) {
        this.linedensity = t;
        const e = Math.floor(this.linedensity * u.width / 100);
        for (; this.linecount > e;) this.linelist.pop(), this.linecount--;
        for (; this.linecount < e;) {
            const t = this.createLine();
            this.linelist.push(t), this.linecount++
        }
    }
}, c.prototype.setLineHeight = function (t) {
    let e = 100 * t / this.linedensity;
    e > 2048 && (e = 2048), e < 64 && (e = 64), this.tipheight = t < 128 ? t / 2 : 64;
    for (let e = 0; e < this.linecount; e++) this.linelist[e].y = this.linelist[e].y + (this.lineheight - t), this.lineheight > t && this.linelist[e].y > u.height && (this.linelist[e].y = 0 - t - (this.linelist[e].y - u.height) - s(0, u.height));
    this.lineheight = t
}, c.prototype.setLineWidth = function (t) {
    t <= 0 || t > 8 || (this.linewidth = t)
}, c.prototype.setLineSpeed = function (t) {
    t != this.linespeed && (this.linespeed = t / 3)
}, c.prototype.setColorSpeed = function (t) {
    t != this.colorspeed && (this.colorspeed = t)
}, c.prototype.setColorBackground = function (t, e, i) {
    const o = (.1 * t / 255).toFixed(3), r = (.1 * e / 255).toFixed(3), n = (.1 * i / 255).toFixed(3);
    this.backgroundColor = [o, r, n]
}, c.prototype.setCustomColor = function (t, e) {
    this.loadedCount++;
    const i = this.colorCustom[t];
    e[0] == i[0] && e[1] == i[1] && e[2] == i[2] || (this.loadedCount > 7 && this.previewColor(e[0], e[1], e[2]), this.colorCustom[t] = e, 7 == this.loadedCount && this.applyColors())
}, c.prototype.applyColors = function () {
    this.colorList = this.colorCustom.slice(), this.colorSpan(!0), this.colorSpan(!1), this.ctime = 0, this.cup = 1
}, c.prototype.previewColor = function (t, e, i) {
    this.preview || (this.previewState.ctime = this.ctime, this.previewState.gradientLength1 = this.gradientLength1, this.previewState.gradientColors1 = this.gradientColors1), this.preview = !0, this.previewState.time = 1, this.ctime = 0, this.gradientLength1 = 1, this.gradientColors1 = [t, e, i]
}, c.prototype.updatePreview = function (t) {
    this.preview && (this.previewState.time -= t, this.previewState.time <= 0 && (this.preview = !1, this.ctime = this.previewState.ctime, this.gradientLength1 = this.previewState.gradientLength1, this.gradientColors1 = this.previewState.gradientColors1, this.applyColors()))
}, f.vertexShaderSource = "#version 300 es\nprecision mediump float;\n\nuniform mat4 modelViewMatrix;\nuniform mat4 projectionMatrix;\nuniform vec3 lineSize;\nuniform vec2 resolution;\n\nlayout(location = 0) in vec3 position;\nlayout(location = 1) in vec4 vertexColor;\nlayout(location = 2) in vec4 offset;\n\nout vec4 varyingVertexColor;\nout float varyingOpacity;\n\nvoid main() {\n\tvaryingVertexColor = vertexColor;\n\tvaryingOpacity = offset.a;\n\tvec4 offsetCentered = offset;\n\toffsetCentered.x -= resolution.x / 2.0;\n\toffsetCentered.y -= resolution.y / 2.0;\n\toffsetCentered.y -= lineSize.y;\n\tvec4 model = vec4(offsetCentered.xyz + position * lineSize, 1.0);\n\t//model.x = clamp(model.x, -resolution.x / 2.0, resolution.x / 2.0);\n\t//model.y = clamp(model.y, -resolution.y / 2.0, resolution.y / 2.0);\n\tvec4 modelViewPosition = modelViewMatrix * model;\n\tgl_Position = projectionMatrix * modelViewPosition;\n}", f.fragmentShaderSource = "#version 300 es\nprecision mediump float;\n\nin vec4 varyingVertexColor;\nin float varyingOpacity;\n\nout vec4 FragColor;\n\nuniform float tipFrac;\nuniform vec2 resolution;\nuniform float blendFactor;\nuniform float gradientLength1;\nuniform float gradientLength2;\nuniform vec3 gradientColors1[5];\nuniform vec3 gradientColors2[5];\n\nfloat map(float v, float f1, float f2, float t1, float t2) { return (v-f1)*(t2-t1)/(f2-f1); }\nfloat min3( vec3 a ) { return min( a.x, min( a.y, a.z ) ); }\nfloat max3( vec3 a ) { return max( a.x, max( a.y, a.z ) ); }\nvec3 mixa( vec3 col1, vec3 col2, float gradient ) { float m = ( max3( col1 ) + max3( col2 ) ) / 2.; vec3 c = ( col1 + col2 ) * .5; float d = 2. * abs( gradient - .5 ) * min3( c ); c = ( c - d ) / ( 1. - d ); c *= m / max3( c ); float s = step( .5, gradient ); gradient *= 2.; return ( 1. - s ) * mix( col1, c, gradient ) + s * mix( c, col2, gradient - 1. ); }\n\nvoid main() {\n\tFragColor = varyingVertexColor;\n\tFragColor.a *= varyingOpacity;\n\n\tfloat div1 = 1.0 / (gradientLength1 - 1.0);\n\tfloat div2 = 1.0 / (gradientLength2 - 1.0);\n\tfloat x = mod(gl_FragCoord.x, resolution.x) / resolution.x;\n\tvec3 color1 = mix(gradientColors1[0],gradientColors1[1], smoothstep(0.0, div1, x));\n\tcolor1 = mix(color1,gradientColors1[2], smoothstep(div1, div1*2.0, x));\n\tcolor1 = mix(color1,gradientColors1[3], smoothstep(div1*2.0, div1*3.0, x));\n\tcolor1 = mix(color1,gradientColors1[4], smoothstep(div1*3.0, div1*4.0, x));\n\tvec3 color2 = mix(gradientColors2[0],gradientColors2[1], smoothstep(0.0, div2, x));\n\tcolor2 = mix(color2,gradientColors2[2], smoothstep(div2, div2*2.0, x));\n\tcolor2 = mix(color2,gradientColors2[3], smoothstep(div2*2.0, div2*3.0, x));\n\tcolor2 = mix(color2,gradientColors2[4], smoothstep(div2*3.0, div2*4.0, x));\n\tFragColor *= vec4(mix(color1/255.0, color2/255.0, blendFactor), 1.0);\n\tfloat tip = clamp(map(varyingVertexColor.a, tipFrac, 1.0, 0.0, 1.0), 0.0, 1.0);\n\tfloat condition = max(sign(varyingOpacity - (200.0 / 255.0)), 0.0);\n\tFragColor.rgb = mix(FragColor.rgb, vec3(1.0), tip * condition);\n\tfloat bottomFade = smoothstep(0.0, resolution.y / 2.0, gl_FragCoord.y);\n\tfloat topFade = smoothstep(resolution.y, resolution.y-resolution.y / 4.0, gl_FragCoord.y);\n\tFragColor.a *= (topFade * 0.7) + 0.3;\n\tFragColor.a = mix(0.01, FragColor.a, bottomFade);\n}", f.particleVertexShaderSource = "#version 300 es\nprecision mediump float;\n\nin vec4 offset;\nin vec3 velocity;\n\nuniform float dt;\nuniform float linespeed;\nuniform vec2 windowsize;\nuniform vec3 linescale;\n\nout vec4 outOffset;\nout vec3 outVelocity;\n\nvoid main() {\n\toutOffset = offset;\n\toutVelocity = velocity;\n\toutOffset += vec4(velocity * linespeed * dt, 0.0);\n\toutOffset.y = mod(outOffset.y, (windowsize.y + linescale.y));\n}", f.discardFragmentShaderSource = "#version 300 es\nprecision mediump float;\nvoid main() {\n\tdiscard;\n}", f.prototype.setup = function () {
    this.shaderProgram = new r(this.gl, f.vertexShaderSource, f.fragmentShaderSource, null), this.particleShaderProgram = new r(this.gl, f.particleVertexShaderSource, f.discardFragmentShaderSource, ["outOffset", "outVelocity"]), r.assemblePrograms();
    const t = new Float32Array([0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1]), e = [{ name: "offset", size: 4 }, { name: "velocity", size: 3 }];
    this.bufferPosition = new n(m, t, [{ name: "position", size: 3 }, { name: "vertexColor", size: 4 }]), this.bufferOffset = new l(() => new n(m, null, e)), this.vaoDraw = new l(() => {
    }), this.vaoDraw.front = a(this.gl, this.shaderProgram, [this.bufferPosition, this.bufferOffset.front], [{
        name: "offset",
        size: 1
    }]), this.vaoDraw.back = a(this.gl, this.shaderProgram, [this.bufferPosition, this.bufferOffset.back], [{ name: "offset", size: 1 }]), this.vaoTransform = new l(() => {
    }), this.vaoTransform.front = a(this.gl, this.particleShaderProgram, [this.bufferOffset.front], null), this.vaoTransform.back = a(this.gl, this.particleShaderProgram, [this.bufferOffset.back], null)
}, f.prototype.update = function (t) {
    this.GLtransformParticles(t)
}, f.prototype.draw = function (t) {
    const e = this.gl;
    this.shaderProgram.activate(), this.shaderProgram.setUniform("projectionMatrix", t.projectionMatrix), this.shaderProgram.setUniform("modelViewMatrix", t.viewMatrix), e.viewport(t.viewPort.x, t.viewPort.y, t.viewPort.width, t.viewPort.height), e.bindVertexArray(this.vaoDraw.front), e.drawArraysInstanced(e.TRIANGLE_STRIP, 0, 4, this.chromad.linecount), e.bindVertexArray(null)
}, f.prototype.updateUniforms = function () {
    this.shaderProgram.setUniform("tipFrac", 1 - this.chromad.tipheight / this.chromad.lineheight), this.shaderProgram.setUniform("lineSize", this.chromad.linewidth, this.chromad.lineheight, 1), this.shaderProgram.setUniform("blendFactor", this.chromad.ctime / 255), this.shaderProgram.setUniform("resolution", this.gl.canvas.width, this.gl.canvas.height), this.shaderProgram.setUniform("gradientLength1", this.chromad.gradientLength1), this.shaderProgram.setUniform("gradientLength2", this.chromad.gradientLength2), this.shaderProgram.setUniform("gradientColors1", this.chromad.gradientColors1), this.shaderProgram.setUniform("gradientColors2", this.chromad.gradientColors2)
}, f.prototype.lineBufferReload = function (t) {
    t && this.getLineState(), this.resetLineState()
}, f.prototype.GLtransformParticles = function (t) {
    const e = this.gl;
    this.chromad.linecount != this.linesInUse && this.lineBufferReload(!0), this.particleShaderProgram.activate(), this.particleShaderProgram.setUniform("dt", t), this.particleShaderProgram.setUniform("linespeed", this.chromad.linespeed), this.particleShaderProgram.setUniform("linescale", this.chromad.linewidth, this.chromad.lineheight, 1), this.particleShaderProgram.setUniform("windowsize", e.canvas.width, e.canvas.height), e.viewport(0, 0, e.drawingBufferWidth, e.drawingBufferHeight), e.bindVertexArray(this.vaoTransform.front), e.bindBufferBase(e.TRANSFORM_FEEDBACK_BUFFER, 0, this.bufferOffset.back.buffer), e.enable(e.RASTERIZER_DISCARD), e.beginTransformFeedback(e.POINTS), e.drawArrays(e.POINTS, 0, this.chromad.linecount), e.endTransformFeedback(), e.disable(e.RASTERIZER_DISCARD), e.bindBufferBase(e.TRANSFORM_FEEDBACK_BUFFER, 0, null), e.bindVertexArray(null), this.vaoTransform.swap(), this.vaoDraw.swap(), this.bufferOffset.swap()
}, f.prototype.getLineState = function () {
    this.bufferOffset.front.getData(this.lineBuffer), this.deserializeLineList()
}, f.prototype.resetLineState = function () {
    this.serializeLineList(), this.bufferOffset.front.setData(this.lineBuffer, 0, 7 * this.chromad.linecount), this.bufferOffset.back.sizeData(this.lineBuffer), this.linesInUse = this.chromad.linecount
}, f.prototype.serializeLineList = function () {
    const t = 8192, e = Math.ceil(7 * this.chromad.linecount / t) * t;
    (!this.lineBuffer || e > this.lineBuffer.length || e < this.lineBuffer.length) && (this.lineBuffer = new Float32Array(e));
    Math.floor(7 * this.chromad.linecount / t);
    const i = this.lineBuffer;
    let o = 0;
    for (let t = 0, e = this.chromad.linecount; t < e; t++) {
        const e = this.chromad.linelist[t];
        i[o++] = e.x, i[o++] = e.y, i[o++] = 0, i[o++] = e.alpha, i[o++] = 0, i[o++] = e.sp, i[o++] = 0
    }
}, f.prototype.deserializeLineList = function () {
    const t = this.lineBuffer;
    let e = 0;
    for (let i = 0, o = Math.min(this.chromad.linecount, this.linesInUse); i < o; i++) {
        const o = this.chromad.linelist[i];
        o.x = t[e++], o.y = t[e++], e++, o.alpha = t[e++], e++, o.sp = t[e++], e++
    }
};
let p, v, y = {
    fps: 0,
    fpslock: !0,
    fpsThreshold: 0,
    totaldt: 0,
    dprscale: !1,
    lastTime: 0,
    lastDt: 0,
    frameID: 0,
    resizeID: null,
    renderer: null,
    language: null,
    camera: new t(0, 0, 0, 0),
    vrcamera: new t(0, 0, 0, 0)
};

function w(t) {
    m = t.getContext("webgl2", {
        antialias: !1,
        preserveDrawingBuffer: !1,
        xrCompatible: !0
    }), m.viewport(0, 0, m.drawingBufferWidth, m.drawingBufferHeight), m.enable(m.BLEND), m.blendFunc(m.SRC_ALPHA, m.ONE_MINUS_SRC_ALPHA), m.clearColor(0, 0, 0, 1), m.clear(m.COLOR_BUFFER_BIT), y.renderer = m.getParameter(m.RENDERER), y.language = m.getParameter(m.SHADING_LANGUAGE_VERSION), v = new f(p, m), v.setup(), R(m), window.addEventListener("resize", B, !1), v.resetLineState()
}

function C() {
    v.updateUniforms(), m.clearColor(p.backgroundColor[0], p.backgroundColor[1], p.backgroundColor[2], 1), m.clearDepth(1), m.clear(m.COLOR_BUFFER_BIT | m.DEPTH_BUFFER_BIT), v.draw(y.camera)
}

function R(t) {
    {
        const i = new Float32Array([1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1]), o = t.canvas.width / 2, r = 180 == p.rotation ? t.canvas.height / 2 : t.canvas.height / -2,
            n = e(-o, o, -r, r, 0, 200);
        y.camera.projectionMatrix = n, y.camera.viewMatrix = i, y.camera.viewPort.x = 0, y.camera.viewPort.y = 0, y.camera.viewPort.width = t.drawingBufferWidth, y.camera.viewPort.height = t.drawingBufferHeight
    }
}

function A() {
    m.bindBuffer(m.ARRAY_BUFFER, v.bufferOffset.front.buffer), console.log("LineCount: " + p.linecount), console.log("LineList: " + p.linelist.length), console.log("Lineinuse: " + v.linesInUse), console.log("Linebuffer size:" + v.lineBuffer.byteLength), console.log("GLBuffer size: " + m.getBufferParameter(m.ARRAY_BUFFER, m.BUFFER_SIZE)), m.bindBuffer(m.ARRAY_BUFFER, null)
}

function b(t) {
    console.error("WebGL Context could not be created.")
}

function F(t) {
    t.preventDefault(), cancelAnimationFrame(y.frameID)
}

function x() {
    const t = y.dprscale && window.devicePixelRatio || 1, e = u.getBoundingClientRect();
    u.width = e.width * t, u.height = e.height * t, r.clearAll(), w(u), L()
}

function S() {
    let t = m.getExtension("WEBGL_lose_context");
    t.loseContext(), window.setTimeout((function (t) {
        t.restoreContext()
    }), 1e3, t)
}

function B() {
    null != y.resizeID && (window.clearTimeout(y.resizeID), y.resizeID = null), y.resizeID = window.setTimeout(E, 250)
}

function E() {
    const t = y.dprscale && window.devicePixelRatio || 1, e = u.getBoundingClientRect();
    u.width = e.width * t, u.height = e.height * t, R(m), p.prepareLines(), v.lineBufferReload(!1)
}

function L() {
    y.lastTime = performance.now() / 1e3, y.frameID = window.requestAnimationFrame(P)
}

function P(t) {
    y.frameID = window.requestAnimationFrame(P);
    const e = performance.now() / 1e3;
    let i = Math.min(e - y.lastTime, 1);
    if (y.lastTime = e, y.totaldt += i, y.fps > 0 && y.fpslock) {
        if (y.fpsThreshold += i, y.fpsThreshold < 1 / y.fps) return;
        y.fpsThreshold -= 1 / y.fps
    }
    p.update(y.totaldt), v.update(y.totaldt), R(m), C(), y.totaldt = 0, y.lastDt = i
}

function _(t) {
    let e = t.split(" ");
    return e = e.map((function (t) {
        return Math.ceil(255 * t)
    })), e
}

function U(t, e) {
    return t[0] == e[0] && t[1] == e[1] && t[2] == e[2]
}

g(), window.wallpaperPropertyListener = {
    applyUserProperties: function (t) {
        if (t.linecount && p.setLineCount(t.linecount.value), t.lineheight && p.setLineHeight(t.lineheight.value), t.linewidth && p.setLineWidth(t.linewidth.value), t.linespeed && p.setLineSpeed(t.linespeed.value), t.colorspeed && p.setColorSpeed(t.colorspeed.value), t.direction && (p.rotation = t.direction.value), t.color0) {
            let e = _(t.color0.value);
            p.setCustomColor(0, e)
        }
        if (t.color1) {
            let e = _(t.color1.value);
            p.setCustomColor(1, e)
        }
        if (t.color2) {
            let e = _(t.color2.value);
            p.setCustomColor(2, e)
        }
        if (t.color3) {
            let e = _(t.color3.value);
            p.setCustomColor(3, e)
        }
        if (t.color4) {
            let e = _(t.color4.value);
            p.setCustomColor(4, e)
        }
        if (t.color5) {
            let e = _(t.color5.value);
            p.setCustomColor(5, e)
        }
        if (t.color6) {
            let e = _(t.color6.value);
            p.setCustomColor(6, e)
        }
        if (t.gradient) {
            let e = t.gradient.value ? 5 : 1;
            p.colorMax != e && (p.colorMax = e, p.applyColors())
        }
        if (t.tint) {
            let e = _(t.tint.value);
            p.setColorBackground(e[0], e[1], e[2])
        }
        t.fpslock && (y.fpslock = t.fpslock.value)
    }, applyGeneralProperties: function (t) {
        t.fps && (y.fps = t.fps)
    }
}, window.wallpaperPluginListener = {
    onPluginLoaded: function (t, e) {
        "led" === t && (T.ledAvailable = !0, T.start())
    }
};
const T = {
    ledAvailable: !1, ctx: null, updateInterval: 100, start: function () {
        T.ledAvailable && (T.setupEffectCanvas(), T.drawEffectCanvas())
    }, setupEffectCanvas: function () {
        let t = document.createElement("canvas");
        t.width = 25, t.height = 6, T.ctx = t.getContext("2d")
    }, drawEffectCanvas: function () {
        const t = T.ctx;
        t.globalAlpha = 1, t.fillStyle = "black", t.fillRect(0, 0, t.canvas.width, t.canvas.height);
        const e = p.gradientColors1, i = p.gradientColors2, o = p.ctime / 255;
        T.colorSpan(t, e, 1), T.colorSpan(t, i, o), T.applyEffectCanvas(), window.setTimeout(T.drawEffectCanvas, T.updateInterval)
    }, applyEffectCanvas: function () {
        const t = T.getEncodedCanvasImageData(T.ctx.canvas);
        window.wpPlugins.led.setAllDevicesByImageData(t, T.ctx.canvas.width, T.ctx.canvas.height)
    }, colorSpan: function (t, e, i) {
        const o = t.canvas.width, r = t.canvas.height;
        let n = o / (e.length / 3 - 1), a = 0;
        var s = t.createLinearGradient(0, 0, o, 0);
        for (let t = 0; t < e.length; t += 3) s.addColorStop(a, "rgb(" + e[t] + "," + e[t + 1] + "," + e[t + 2] + ")"), a += n / o;
        t.globalAlpha = i, t.fillStyle = s, t.fillRect(0, 0, o, r)
    }, getEncodedCanvasImageData: function (t) {
        for (var e = t.getContext("2d").getImageData(0, 0, t.width, t.height), i = [], o = 0; o < e.data.length; o += 4) {
            var r = o / 4 * 3;
            i[r] = e.data[o], i[r + 1] = e.data[o + 1], i[r + 2] = e.data[o + 2]
        }
        return String.fromCharCode.apply(null, i)
    }
};
