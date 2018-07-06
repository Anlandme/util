/**
 * @brief ��Բ��
 *
 * @api publish
 */
function initCanvas() {
    var canvasElements = document.getElementsByTagName('canvas');//����canvas
    width  = 100;// canvas�����
    height = 100;// canvas,�߶�

    for  (var i = 0; i < canvasElements.length; i++) {
        canvasElements[i].height = height*2;
        canvasElements[i].width  = width*2;
        var value = canvasElements[i].getAttribute('percent');
        draw(canvasElements[i], value);
    }

    /* ����Բ��*/
    function draw(canvas, value) {
        var arr=value.split("/");//�ָ��ַ���
        var precent = parseFloat(arr[1].substr(0, arr[1].length-1));//�����ٷֺŲ�ת��Ϊ��ֵ
        var ctx = canvas.getContext("2d");
        var precent = precent/50;
        if( precent < 0.5){precent = 1.5 + precent;}else{precent = precent - 0.5;}//ת���ٷֱ�

        //��ӰЧ��
        ctx.shadowOffsetX = 10; // ��ӰY��ƫ��
        ctx.shadowOffsetY = 10; // ��ӰX��ƫ��
        ctx.shadowBlur = 10; // ģ���ߴ�
        ctx.shadowColor = 'rgba(0, 0, 0, 0.5)'; // ��ɫ
        //��ͼ
        ctx.beginPath();
        ctx.arc(width, height, height-width/5, 0, 2 * Math.PI);//x���꣬ y���꣬ �뾶�� ��ʼ�ǣ� ������
        ctx.lineWidth = width/5;//Բ�����
        ctx.strokeStyle = '#c0392b'; //�ߵ���ɫ 
        ctx.stroke();
        ctx.beginPath();
        ctx.arc(width, height, height-width/5, 1.5 *Math.PI, (precent-0.0001) * Math.PI);//x���꣬ y���꣬ �뾶�� ��ʼ�ǣ� ������
        ctx.lineWidth = width/5;//Բ�����
        ctx.strokeStyle = '#fbc02d'; //�ߵ���ɫ 
        //ctx.lineCap = "round";//����Բñ����
        ctx.stroke();
        //����
        ctx.fillStyle = '#fbc02d';  // ��ɫ
        ctx.textAlign = 'center';  // λ��
        ctx.font = "lighter 18pt KaiTi";  // �����С����ʽ
        ctx.fillText(value, width, height+10);
    }

    }

//��Բ��
initCanvas();