/**
 * @brief �����ǩp 
 *
 * @api publish
 */
function initTagP() {
  var pElements = document.getElementsByTagName('p');//����p��ǩ
  var liElements = document.getElementsByTagName('li');//����li��ǩ
  var aElements = document.getElementsByTagName('a');//����a��ǩ

  /* ��굥�� �����¼� */
  function clickTagP() {
      var pElement = this.nextElementSibling;
      if ('BR' == pElement.nodeName) {pElement = pElement.nextElementSibling;}//����br��Ѱ����һ���ֵ�Ԫ��

      if ('P' == pElement.nodeName) {
          if ('none' == pElement.style.display) {
              pElement.style.display = '';//��ǰp��ǩ��
              if ('LI' == this.nodeName) { pElement.nextElementSibling.style.padding = '10px 0px 0px 0px'; }
          }
          else {
              pElement.style.display = 'none';//��ǰp��ǩ����
              if ('LI' == this.nodeName) { pElement.nextElementSibling.style.padding = '40px 0px 0px 0px'; }
          }
      }

  }


  for (var j = 0; j < aElements.length; j++) {
      aElements[j].onclick = clickTagP;//��ȫ��a��ǩ��ʼ����굥���¼�
  }

  for (var j = 0; j < liElements.length; j++) {
      liElements[j].onclick = clickTagP;//��ȫ��li��ǩ��ʼ����굥���¼�
  }

  for (var i = 0; i < pElements.length; i++) {
      pElements[i].style.display= 'none';//��ȫ��p��ǩ����
  }
}

//�����ǩP
initTagP();