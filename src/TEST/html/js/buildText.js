/**
 * @brief 处理标签p 
 *
 * @api publish
 */
function initTagP() {
  var pElements = document.getElementsByTagName('p');//所有p标签
  var liElements = document.getElementsByTagName('li');//所有li标签
  var aElements = document.getElementsByTagName('a');//所有a标签

  /* 鼠标单击 处理事件 */
  function clickTagP() {
      var pElement = this.nextElementSibling;
      if ('BR' == pElement.nodeName) {pElement = pElement.nextElementSibling;}//等于br，寻找下一个兄弟元素

      if ('P' == pElement.nodeName) {
          if ('none' == pElement.style.display) {
              pElement.style.display = '';//当前p标签打开
              if ('LI' == this.nodeName) { pElement.nextElementSibling.style.padding = '10px 0px 0px 0px'; }
          }
          else {
              pElement.style.display = 'none';//当前p标签屏蔽
              if ('LI' == this.nodeName) { pElement.nextElementSibling.style.padding = '40px 0px 0px 0px'; }
          }
      }

  }


  for (var j = 0; j < aElements.length; j++) {
      aElements[j].onclick = clickTagP;//将全部a标签初始化鼠标单击事件
  }

  for (var j = 0; j < liElements.length; j++) {
      liElements[j].onclick = clickTagP;//将全部li标签初始化鼠标单击事件
  }

  for (var i = 0; i < pElements.length; i++) {
      pElements[i].style.display= 'none';//将全部p标签屏蔽
  }
}

//处理标签P
initTagP();