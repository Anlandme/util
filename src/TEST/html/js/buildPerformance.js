/**
 * @brief �����ǩlabel
 *
 * @api publish
 */
function initTagLabel() {
  var Element = document.getElementsByTagName('label');
  var change =1;//�ı�0=�������ܱ� ����1=������ϵ��

  /* ��굥�� �����¼� */
  function clickTagLabel() {
      if ( 0 == change) {
          document.getElementById('fun-performance').style.display = '';//չʾ�������ܱ�
          document.getElementById('fun-relation').style.display = 'none';//�رպ�����ϵ��
      }
      else {
          document.getElementById('fun-relation').style.display = '';//չʾ������ϵ��
          document.getElementById('fun-performance').style.display = 'none';//�رպ������ܱ�
      }
      change++;
      change = change%2;
  }

    document.getElementById('fun-performance').style.display = '';//չʾ�������ܱ�
    document.getElementById('fun-relation').style.display = 'none';//�رպ�����ϵ��
    Element[0].onclick = clickTagLabel;//��ʼ����굥���¼�
  }

//�����ǩlabel
initTagLabel();