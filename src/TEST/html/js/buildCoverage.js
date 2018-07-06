/**
 * @brief 处理标签td
 *
 * @api publish
 */
function initTagTd() {
  var trElements = document.getElementsByClassName('comment');//所有class是comment的tr标签

  /* 鼠标单击 处理事件 */
  function clickTagTd() {
      var trElement = this.nextElementSibling;

          if ('none' == trElement.style.display) {
              trElement.style.display = '';//当前class是comment的tr标签打开
          }
          else {
              trElement.style.display = 'none';//当前class是comment的tr标签屏蔽
          }

  }

  for (var j = 0; j < trElements.length; j++) {
        var preTrElement = trElements[j].previousElementSibling;//上一个兄弟节点
        trElements[j].style.display = 'none';//将class是comment的tr标签，屏蔽
        preTrElement.onclick = clickTagTd;//初始化鼠标单击事件
        preTrElement.lastChild.style.color = '#c0392b';//将修改颜色
  }

  }

//处理标签td
initTagTd();