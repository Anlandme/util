/**
 * @brief �����ǩtd
 *
 * @api publish
 */
function initTagTd() {
  var trElements = document.getElementsByClassName('comment');//����class��comment��tr��ǩ

  /* ��굥�� �����¼� */
  function clickTagTd() {
      var trElement = this.nextElementSibling;

          if ('none' == trElement.style.display) {
              trElement.style.display = '';//��ǰclass��comment��tr��ǩ��
          }
          else {
              trElement.style.display = 'none';//��ǰclass��comment��tr��ǩ����
          }

  }

  for (var j = 0; j < trElements.length; j++) {
        var preTrElement = trElements[j].previousElementSibling;//��һ���ֵܽڵ�
        trElements[j].style.display = 'none';//��class��comment��tr��ǩ������
        preTrElement.onclick = clickTagTd;//��ʼ����굥���¼�
        preTrElement.lastChild.style.color = '#c0392b';//���޸���ɫ
  }

  }

//�����ǩtd
initTagTd();