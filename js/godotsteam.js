// Code toggle
$("#gdscript").on("click", function(){
	$(".gdscript").css("display", "inline");
	$(".c-sharp").css("display", "none");
});

$("#c-sharp").on("click", function(){
	$(".gdscript").css("display", "none");
	$(".c-sharp").css("display", "inline");
});

// Scroll to top
$(window).scroll(function() {
    if ($(this).scrollTop()) {
        $('#to-top').show();
    } else {
        $('#to-top').hide();
    }
});

$("#to-top").click(function() {
    $("html, body").animate({scrollTop: 0}, 1000);
 });