/* 网页滚动布局
*|-------------------------totalWidth---------------------------------|                                 
***********************************************************************
*                 |                        |                          *
*                 |                        |                          *         
*                 |--------winWidth--------|                          *
*                 |                        |                          *
*-----offLeft-----|                        |--------offRight----------*
*                 |                        |                          *
***********************************************************************
*/
/**
 * @brief 鼠标滚动操作
 * @api publish
 */
function mousewheel(element) {
    "use strict";

    var panels = document.getElementById(element);
    var totalWidth = panels.scrollWidth;//整个网页宽度
    var winWidth = document.body.offsetWidth;//窗口宽度
    var offLeft = 0;//窗口相对于网页的左边偏移宽度
    var offRight = totalWidth-winWidth-offLeft;//窗口相对于网页的右边偏移宽度
    
    /** @brief 浏览器窗口改变时候重新计算值*/
    function resize() {
        totalWidth = panels.scrollWidth;
        winWidth =document.body.offsetWidth;
        offRight = totalWidth - winWidth - offLeft;
        panels.style.right = offLeft + 'px';
    }

    /** @brief 根据位移值移动窗口 */
    function moveWindow(position) {
        var pos=0;
        var posRight=0;
        var posLeft=0;

        //位移调整
        if ( (120) == position || (-120) == position ){//IE/Opera/Chrome
            position = position / 2;
        } else if ( (3) == position || (-3) == position ){//Firefox
            position = position * 20;
        } 

        if (position < 0) {//屏幕向右偏移
            pos = -(position);
            (offRight - pos > 0) ? offLeft = offLeft + pos : offLeft = offLeft + offRight;//offLeft 增加
            (offRight - pos > 0) ? offRight = offRight - pos : offRight = 0;//offRight 减少
        } else if (position > 0) {//屏幕向左偏移
            pos = position;
            (offLeft - pos > 0) ? offRight = offRight + pos : offRight = offRight + offLeft;//offRight 增加
            (offLeft - pos > 0) ? offLeft = offLeft - pos : offLeft = 0;//offLeft 减少
        }

        panels.style.right = offLeft + 'px';
    }

    /** @brief 鼠标滚动是处理事件函数*/
    function scroll() {
        var offSet;//滚动位移
        var event = window.event;

        if (event.wheelDelta) {//IE/Opera/Chrome  wheelDelta只取±120，其中正数表示为向左，负数表示向右。  
            offSet = event.wheelDelta;
        } else if (event.detail) {//Firefox  detail只取±3,其中正数表示为向左，负数表示向右。  
            offSet = event.detail;
        }
        moveWindow(offSet);
    }

    document.body.onresize = resize; //当浏览器窗口调整时，及时调整位移
    return {
        scroll: function () {scroll();}
    };
}
var fn = mousewheel('panels');
document.getElementById('panels').addEventListener('wheel', fn.scroll); 