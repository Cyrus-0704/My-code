/**
 * 咸鱼万事屋网站JavaScript功能
 */

// 服务页面选项卡切换功能
function openService(serviceName) {
    const contents = document.getElementsByClassName("service-content");
    for (let i = 0; i < contents.length; i++) {
        contents[i].style.display = "none";
    }
    
    const btns = document.getElementsByClassName("tab-btn");
    for (let i = 0; i < btns.length; i++) {
        btns[i].className = btns[i].className.replace(" active", "");
    }
    
    document.getElementById(serviceName).style.display = "block";
    event.currentTarget.className += " active";
}

// 联系页面表单提交处理
document.addEventListener('DOMContentLoaded', function() {
    const contactForm = document.getElementById('contactForm');
    if (contactForm) {
        contactForm.addEventListener('submit', function(event) {
            event.preventDefault();
            
            const name = document.getElementById('name').value;
            const message = document.getElementById('message').value;
            
            // 这里应该是表单提交到服务器的代码
            // 由于是静态网站，这里只做简单处理
            
            document.getElementById('formStatus').textContent = `谢谢您的留言，${name}！我们会尽快回复。`;
            contactForm.reset();
        });
    }
});
