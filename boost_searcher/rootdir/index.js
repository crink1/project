function Search(){
    // 是浏览器的一个弹出框
    // alert("hello js!");
    // 1. 提取数据, $可以理解成就是JQuery的别称
    let query = $(".container .search input").val();
    console.log("query = " + query); //console是浏览器的对话框，可以用来进行查看js数据

    //2. 发起http请求,ajax: 属于一个和后端进行数据交互的函数，JQuery中的
    $.ajax({
        type: "GET",
        url: "/s?word=" + query,
        success: function(data){
            console.log(data);
            BuildHtml(data);
        }
    });
}

function BuildHtml(data){
    // 获取html中的result标签
    let result_lable = $(".container .result");
    // 清空历史搜索结果
    result_lable.empty();

    for( let elem of data){
        // console.log(elem.title);
        // console.log(elem.url);
        let a_lable = $("<a>", {
            text: elem.title,
            href: elem.url,
            // 跳转到新的页面
            target: "_blank"
        });
        let p_lable = $("<p>", {
            text: elem.desc
        });
        let i_lable = $("<i>", {
            text: elem.url
        });
        let div_lable = $("<div>", {
            class: "item"
        });
        a_lable.appendTo(div_lable);
        p_lable.appendTo(div_lable);
        i_lable.appendTo(div_lable);
        div_lable.appendTo(result_lable);
    }
}