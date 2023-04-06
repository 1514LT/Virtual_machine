function cancel() {
    document.getElementById("username").value = "";
    document.getElementById("password").value = "";
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
function login() {
    //获取用户名和密码
    var usr = document.getElementById("username").value;
    var pwd = document.getElementById("password").value;
    if (usr == "" || pwd == "") {
        alert("请输入用户名密码");
        return;
    }
    //1、创建xmlhttp对象
    var xmlhttp = getXMLHttpRequest();
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            //处理服务器的应答 xmlhttp.responseText
            var ret = xmlhttp.responseText;
            // alert("##" + ret + "##");
            if (ret == "1") {
                // alert("----跳转网页---");
                window.location.href = "./total.html";
            }
            else if (ret == "0") {
                alert("----用户密码错误---");
            }
        }
        //else { alert("erro"); return; }
    }
    var url = "./cgi-bin/login/login.cgi?";
    url += usr;
    url += ":";
    url += pwd;
    // alert("url" + url);
    xmlhttp.open("GET", url, true);
    //4、发送请求
    xmlhttp.send();

}
