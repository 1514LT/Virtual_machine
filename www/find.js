function cancel() {
    document.getElementById("username").value = "";
    document.getElementById("usercard").value = "";
}
function back() {
    window.location.href = "./total.html";
}
function showdata(str) {
    s1 = str.split('.');
    var row = parseInt(s1[0]);
    var table = document.getElementById("table");
    //alert("table="+table);
    var tmplen = table.rows.length;
    //alert("tmplen="+tmplen);
    for (var i = 1; i < tmplen; i++) {
        table.deleteRow(i);
    }
    var tmpstr = new Array();
    tmpstr = s1[1].split(" ");
    if (str.length > 0) {
        for (var i = 0; i < row; i++) {
            var x = table.insertRow(table.rows.length)
            var y = x.insertCell(0);
            var z = x.insertCell(1);
            var q = x.insertCell(2);
            y.innerHTML = tmpstr[i * 3];
            z.innerHTML = tmpstr[i * 3 + 1];
            q.innerHTML = tmpstr[i * 3 + 2];
        }

    }
}
function getXMLHttpRequest() {
    var xmlhttp = null;
    if (window.XMLHttpRequest)//自动检测当前浏览器的版本，如果是 IE5.0 以上的高版本的浏览器
    {// code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp = new XMLHttpRequest();//创建请求对象
    }
    else////如果浏览器是底版本的
    {// code for IE6, IE5
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");//创建请求对象
    }
    return xmlhttp;//返回请求对象
}
function find() {
    var name = document.getElementById("username").value;
    var card = 0;
    //var card = document.getElementById("usercard").value;
    var choise_name = document.getElementById("choise_name").value;
    var choise_id = document.getElementById("choise_card").value;
    var xmlhttp = getXMLHttpRequest();
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            //处理服务器的应答 xmlhttp.responseText
            var ret = xmlhttp.responseText;
            alert(ret);
            if (ret != "0") {
                alert("----查找成功---");
                //window.location.href = "./total.html";
                document.getElementById('div').innerHTML = ret;
            }
            else if (ret == "0") {
                alert("----查找失败---");
            }
        }
    }
    var url = "./cgi-bin/find/find.cgi?";
    url += name;
    url += ":";
    url += card + ":" + choise_name + ":" + choise_id;
    alert("url" + url);
    xmlhttp.open("GET", url, true);
    //4、发送请求
    xmlhttp.send();
}
