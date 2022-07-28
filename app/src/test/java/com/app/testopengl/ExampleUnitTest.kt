package com.app.testopengl

import org.junit.Test

import org.junit.Assert.*

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
class ExampleUnitTest {
    @Test
    fun addition_isCorrect() {
        val flag1 = 0b0010
        var res = 0b0000
        res = res or flag1
        println(res)
        res = res xor flag1
        println(res)

        assertEquals(4, 2 + 2)
    }
}