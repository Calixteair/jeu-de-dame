
$(window).scroll(function (event) {
  var y = $(this).scrollTop();

  if (y >= 160) {
    $('.headfixe').addClass('fixed');
  } else {
    $('.headfixe').removeClass('fixed');
  }
  if(y >= 260){
    $('.opacity').addClass('fixed');
  }
  else {
    $('.opacity').removeClass('fixed');
  }
  if(y >= 270){
    $('.scrollUp').addClass('visible');
  }
  else {
    $('.scrollUp').removeClass('visible');
  }

});
