const progressbar = document.querySelector('progress');
const article = document.querySelector('div.corps');



let isScrolling = false;

document.addEventListener('scroll', e => isScrolling = true);
render();


function render() {

  requestAnimationFrame(render);

  if (!isScrolling) return;

  progressbar.value = (window.scrollY - 200)   / (article.offsetHeight - window.innerHeight) * 100;

  isScrolling = false;

}

const allRonds = document.querySelectorAll('.date');
const allBoxes = document.querySelectorAll('.box');

const controller = new ScrollMagic.Controller()




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



})


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
