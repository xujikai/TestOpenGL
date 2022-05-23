package com.app.testopengl.bean

data class MotionBean(
    var totalTime: Double = 0.0,
    var curves: List<MotionCurvesBean> = mutableListOf(),
    var Hips: MotionHipBean = MotionHipBean()
)

data class MotionCurvesBean(
    var name: String = "",
    var path: String = "",
    var rotas: List<MotionRotaBean> = mutableListOf(),
)

data class MotionRotaBean(
    var x: Double,
    var y: Double,
    var z: Double,
    var w: Double,
)

data class MotionHipBean(
    var name: String = "",
    var path: String = "",
    var pos: List<MotionHipPosBean> = mutableListOf(),
)

data class MotionHipPosBean(
    var x: Double,
    var y: Double,
    var z: Double,
)