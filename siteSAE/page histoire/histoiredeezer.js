const progressbar = document.querySelector('progress');
const article = document.querySelector('div.corps');
const allRonds = document.querySelectorAll('.date');
const allBoxes = document.querySelectorAll('.box');
const head2 = document.querySelector('.head2');
let thead =  head2.offsetHeight;

const controller = new ScrollMagic.Controller();




$(function(){
  if (window.matchMedia("(min-width: 1000px)").matches) {
    $('.menu').removeClass('dropdown'),
    $('.boxhead').removeClass('dropdownchild');


    $(window).scroll(function (event) {
      var y = $(this).scrollTop();
    
      if (y >= thead) {
        $('.headfixe').addClass('fixed');
      } else {
        $('.headfixe').removeClass('fixed');
      }
      if(y >= (thead + 100)){
        $('.opacity').addClass('fixed');
      }
      else {
        $('.opacity').removeClass('fixed');
      }
      if(y >= (thead + 110)){
        $('.scrollUp').addClass('visible');
      }
      else {
        $('.scrollUp').removeClass('visible');
      }
    });

  }else{
    $('.menu').addClass('dropdown');
    $('.boxhead').addClass('dropdownchild');

    $(window).scroll(function (event) {
      var y = $(this).scrollTop();

      if(y >= (thead + ((window.innerHeight*10)/100))){
        $('.opacity').addClass('fixed2');
      }
      else {
        $('.opacity').removeClass('fixed2');
      }
    });

  }
});


$(function(){
  if (window.matchMedia("(min-width: 1000px)").matches) {

    let isScrolling = false;
  
          document.addEventListener('scroll', e => isScrolling = true);
          render();
  
  
          function render() {
  
            requestAnimationFrame(render);
  
            if (!isScrolling) return;
  
            progressbar.value = (window.scrollY - 200)   / (article.offsetHeight - window.innerHeight) * 100;
  
            isScrolling = false;
  
          }
  
          allBoxes.forEach(box => {
  
              for(i = 0; i < allRonds.length; i++){
  
                  if(allRonds[i].getAttribute('data-anim') === box.getAttribute('data-anim')){
  
                      let tween = gsap.from(box, {y: -50, opacity: 0, duration: 0.5})
  
                      let scene = new ScrollMagic.Scene({
                          triggerElement: allRonds[i],
                          reverse: true
                      })
                      .setTween(tween)
                      .addIndicators()
                      .addTo(controller)
                  }
  
              }
  
  
  
          });

          
  
  }

 else {
  let tween = gsap.from(box, {y: -50, opacity: 0, duration: 0.5})
  .setTween(tween)
  progressbar.value = 100
 }
    
});




