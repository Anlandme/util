/**
 * @brief 画圆环
 *
 * @api publish
 */
function initCanvas() {
    var canvasElements = document.getElementsByTagName('canvas');//所有canvas
    width  = 100;// canvas，宽度
    height = 100;// canvas,高度

    for  (var i = 0; i < canvasElements.length; i++) {
        canvasElements[i].height = height*2;
        canvasElements[i].width  = width*2;
        var value = canvasElements[i].getAttribute('percent');
        draw(canvasElements[i], value);
    }

    /* 画出圆环*/
    function draw(canvas, value) {
        var arr=value.split("/");//分割字符串
        var precent = parseFloat(arr[1].substr(0, arr[1].length-1));//消除百分号并转化为数值
        var ctx = canvas.getContext("2d");
        var precent = precent/50;
        if( precent < 0.5){precent = 1.5 + precent;}else{precent = precent - 0.5;}//转化百分比

        //阴影效果
        ctx.shadowOffsetX = 10; // 阴影Y轴偏移
        ctx.shadowOffsetY = 10; // 阴影X轴偏移
        ctx.shadowBlur = 10; // 模糊尺寸
        ctx.shadowColor = 'rgba(0, 0, 0, 0.5)'; // 颜色
        //画图
        ctx.beginPath();
        ctx.arc(width, height, height-width/5, 0, 2 * Math.PI);//x坐标， y坐标， 半径， 起始角， 结束角
        ctx.lineWidth = width/5;//圆环宽度
        ctx.strokeStyle = '#c0392b'; //边的颜色 
        ctx.stroke();
        ctx.beginPath();
        ctx.arc(width, height, height-width/5, 1.5 *Math.PI, (precent-0.0001) * Math.PI);//x坐标， y坐标， 半径， 起始角， 结束角
        ctx.lineWidth = width/5;//圆环宽度
        ctx.strokeStyle = '#fbc02d'; //边的颜色 
        //ctx.lineCap = "round";//边是圆帽类型
        ctx.stroke();
        //字体
        ctx.fillStyle = '#fbc02d';  // 颜色
        ctx.textAlign = 'center';  // 位置
        ctx.font = "lighter 18pt KaiTi";  // 字体大小，样式
        ctx.fillText(value, width, height+10);
    }

    }

//画圆环
initCanvas();