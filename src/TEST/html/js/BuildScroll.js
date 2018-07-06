/* ��ҳ��������
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
 * @brief ����������
 * @api publish
 */
function mousewheel(element) {
    "use strict";

    var panels = document.getElementById(element);
    var totalWidth = panels.scrollWidth;//������ҳ���
    var winWidth = document.body.offsetWidth;//���ڿ��
    var offLeft = 0;//�����������ҳ�����ƫ�ƿ��
    var offRight = totalWidth-winWidth-offLeft;//�����������ҳ���ұ�ƫ�ƿ��
    
    /** @brief ��������ڸı�ʱ�����¼���ֵ*/
    function resize() {
        totalWidth = panels.scrollWidth;
        winWidth =document.body.offsetWidth;
        offRight = totalWidth - winWidth - offLeft;
        panels.style.right = offLeft + 'px';
    }

    /** @brief ����λ��ֵ�ƶ����� */
    function moveWindow(position) {
        var pos=0;
        var posRight=0;
        var posLeft=0;

        //λ�Ƶ���
        if ( (120) == position || (-120) == position ){//IE/Opera/Chrome
            position = position / 2;
        } else if ( (3) == position || (-3) == position ){//Firefox
            position = position * 20;
        } 

        if (position < 0) {//��Ļ����ƫ��
            pos = -(position);
            (offRight - pos > 0) ? offLeft = offLeft + pos : offLeft = offLeft + offRight;//offLeft ����
            (offRight - pos > 0) ? offRight = offRight - pos : offRight = 0;//offRight ����
        } else if (position > 0) {//��Ļ����ƫ��
            pos = position;
            (offLeft - pos > 0) ? offRight = offRight + pos : offRight = offRight + offLeft;//offRight ����
            (offLeft - pos > 0) ? offLeft = offLeft - pos : offLeft = 0;//offLeft ����
        }

        panels.style.right = offLeft + 'px';
    }

    /** @brief �������Ǵ����¼�����*/
    function scroll() {
        var offSet;//����λ��
        var event = window.event;

        if (event.wheelDelta) {//IE/Opera/Chrome  wheelDeltaֻȡ��120������������ʾΪ���󣬸�����ʾ���ҡ�  
            offSet = event.wheelDelta;
        } else if (event.detail) {//Firefox  detailֻȡ��3,����������ʾΪ���󣬸�����ʾ���ҡ�  
            offSet = event.detail;
        }
        moveWindow(offSet);
    }

    document.body.onresize = resize; //����������ڵ���ʱ����ʱ����λ��
    return {
        scroll: function () {scroll();}
    };
}
var fn = mousewheel('panels');
document.getElementById('panels').addEventListener('wheel', fn.scroll); 