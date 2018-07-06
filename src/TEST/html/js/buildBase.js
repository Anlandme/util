/**
 * @brief 鼠标经过触发事件
 *
 * @param x 鼠标所在的div块
 * @api publish
 */
function MouseArrive(x) {
  var classElements = [], allElements = document.getElementsByTagName('span');//所有span
  for (var i = 0; i < allElements.length; i++) {
    allElements[i].style.filter = 'brightness(0.6)';//全部变暗
  }
  x.style.filter = 'brightness(1)';//鼠标经过的块变亮
}
/**
 * @brief 鼠标离开触发事件
 *
 * @param x 鼠标所在的div块
 * @api publish
 */
function MouseLeave(x) {
  var classElements = [], allElements = document.getElementsByTagName('span');//所有span
  for (var i = 0; i < allElements.length; i++) {
    allElements[i].style.filter = 'brightness(1)';//恢复全部亮
  }
}



/**
 * @brief 计算div数量class名字为n的数量 
 *
 * @param name div的名字
 * @api publish
 */
function getElementsByClassName(name) {
  var classElements = [], allElements = document.getElementsByTagName('div');//所有div
  for (var i = 0; i < allElements.length; i++) {
    if (allElements[i].className == name) {
      classElements[classElements.length] = allElements[i];
    }
  }
  return classElements.length;
}

//计算新的panels的width
panels = document.getElementById('panels');
panels.style.width = 300 * (getElementsByClassName('panel-text')+1) + 'px';
