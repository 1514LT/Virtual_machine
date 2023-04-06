function clean() {
    //document.getElementById("username").value = "";
    document.getElementById("key").value = "";
    document.getElementById("password").value = "";
    document.getElementById("password_agin").value = "";
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
function sing_up() {
    var key = document.getElementById("key").value;
    var username = document.getElementById("username").value;
    var password = document.getElementById("password").value;
    var password_agin = document.getElementById("password_agin").value;
    if (key == "") {
        alert("key值不能为空!");
        document.getElementById("key").value = "";
    }
    if (key != "1514") { alert("请输入正确的key值"); return Error; }
        
    if (username == "" || password == "" || password_agin == "") {
        alert("账户密码不能为空！");
        clean();
    }

    else if (password != password_agin) {
        alert("请输入相同的密码！");
        clean();
    }

    //1、创建xmlhttp对象
    var xmlhttp = getXMLHttpRequest();
    //alert(xmlhttp);
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            //处理服务器的应答 xmlhttp.responseText
            var ret = xmlhttp.responseText;
            alert("##" + ret + "##");
            if (ret == "1") {
                alert("----注册成功---");
            }
            else if (ret == "0") {
                alert("----当前卡号已注册---");
            }
            // else { 
            //     alert("注册失败");
            // }
        }
        // else { alert("erro"); return; }
    }
    var url = "./cgi-bin/singup/singup.cgi?";
    //alert(url);
    url += username;
    url += ":";
    //alert(url);
    url += password;
    //url += ":"+password;//获取卡号，组包到url中
    //alert(url);
    //?????????????????????????????????card
    alert(url);
    xmlhttp.open("GET", url, true);
    //4、发送请求
    xmlhttp.send();
    clean();
}
