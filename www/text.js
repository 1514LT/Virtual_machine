function ok() {
    //获取所有单选按钮（同一组），得到对象
    var fruits = document.getElementById("1");
    for (var i = 0; i < fruits.length; i++) {

        if (fruits[i].checked) {
            alert("选择了" + fruits[i].value);
        }
    }
}
