/**
 * @brief ��꾭�������¼�
 *
 * @param x ������ڵ�div��
 * @api publish
 */
function MouseArrive(x) {
  var classElements = [], allElements = document.getElementsByTagName('span');//����span
  for (var i = 0; i < allElements.length; i++) {
    allElements[i].style.filter = 'brightness(0.6)';//ȫ���䰵
  }
  x.style.filter = 'brightness(1)';//��꾭���Ŀ����
}
/**
 * @brief ����뿪�����¼�
 *
 * @param x ������ڵ�div��
 * @api publish
 */
function MouseLeave(x) {
  var classElements = [], allElements = document.getElementsByTagName('span');//����span
  for (var i = 0; i < allElements.length; i++) {
    allElements[i].style.filter = 'brightness(1)';//�ָ�ȫ����
  }
}



/**
 * @brief ����div����class����Ϊn������ 
 *
 * @param name div������
 * @api publish
 */
function getElementsByClassName(name) {
  var classElements = [], allElements = document.getElementsByTagName('div');//����div
  for (var i = 0; i < allElements.length; i++) {
    if (allElements[i].className == name) {
      classElements[classElements.length] = allElements[i];
    }
  }
  return classElements.length;
}

//�����µ�panels��width
panels = document.getElementById('panels');
panels.style.width = 300 * (getElementsByClassName('panel-text')+1) + 'px';
