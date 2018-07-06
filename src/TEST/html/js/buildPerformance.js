/**
 * @brief 处理标签label
 *
 * @api publish
 */
function initTagLabel() {
  var Element = document.getElementsByTagName('label');
  var change =1;//改变0=函数性能表 或者1=函数关系表

  /* 鼠标单击 处理事件 */
  function clickTagLabel() {
      if ( 0 == change) {
          document.getElementById('fun-performance').style.display = '';//展示函数性能表
          document.getElementById('fun-relation').style.display = 'none';//关闭函数关系表
      }
      else {
          document.getElementById('fun-relation').style.display = '';//展示函数关系表
          document.getElementById('fun-performance').style.display = 'none';//关闭函数性能表
      }
      change++;
      change = change%2;
  }

    document.getElementById('fun-performance').style.display = '';//展示函数性能表
    document.getElementById('fun-relation').style.display = 'none';//关闭函数关系表
    Element[0].onclick = clickTagLabel;//初始化鼠标单击事件
  }

//处理标签label
initTagLabel();