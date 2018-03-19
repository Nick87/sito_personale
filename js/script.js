function goToByScroll(id)
{
	var offset;
	if(id == "#cv")
		offset = 0;
	else {
		if($(".navigation").hasClass("main-nav-fixed"))
			offset = $(id).offset().top - 40;
		else
			offset = $(id).offset().top - 40;
	}
	$('html,body').animate({scrollTop:offset}, 500);
}

function viewport()
{
	var e = window, a = 'inner';
	if ( !( 'innerWidth' in window ) ) {
		a = 'client';
		e = document.documentElement || document.body;
	}
	return { width : e[ a+'Width' ] , height : e[ a+'Height' ] };
}

function isScrolledIntoView(elem)
{
	var docViewTop = $(window).scrollTop();
	var docViewBottom = docViewTop + $(window).height();
	var elemTop = $(elem).offset().top;
	var elemBottom = elemTop + $(elem).height();

	return ((elemBottom <= docViewBottom) && (elemTop >= docViewTop));
}

function checkMobile()
{
	var ua = navigator.userAgent || navigator.vendor || window.opera;
	return /(android|bb\d+|meego).+mobile|avantgo|bada\/|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od)|iris|kindle|lge |maemo|midp|mmp|netfront|opera m(ob|in)i|palm( os)?|phone|p(ixi|re)\/|plucker|pocket|psp|series(4|6)0|symbian|treo|up\.(browser|link)|vodafone|wap|windows (ce|phone)|xda|xiino/i.test(ua) ||
		   /1207|6310|6590|3gso|4thp|50[1-6]i|770s|802s|a wa|abac|ac(er|oo|s\-)|ai(ko|rn)|al(av|ca|co)|amoi|an(ex|ny|yw)|aptu|ar(ch|go)|as(te|us)|attw|au(di|\-m|r |s )|avan|be(ck|ll|nq)|bi(lb|rd)|bl(ac|az)|br(e|v)w|bumb|bw\-(n|u)|c55\/|capi|ccwa|cdm\-|cell|chtm|cldc|cmd\-|co(mp|nd)|craw|da(it|ll|ng)|dbte|dc\-s|devi|dica|dmob|do(c|p)o|ds(12|\-d)|el(49|ai)|em(l2|ul)|er(ic|k0)|esl8|ez([4-7]0|os|wa|ze)|fetc|fly(\-|_)|g1 u|g560|gene|gf\-5|g\-mo|go(\.w|od)|gr(ad|un)|haie|hcit|hd\-(m|p|t)|hei\-|hi(pt|ta)|hp( i|ip)|hs\-c|ht(c(\-| |_|a|g|p|s|t)|tp)|hu(aw|tc)|i\-(20|go|ma)|i230|iac( |\-|\/)|ibro|idea|ig01|ikom|im1k|inno|ipaq|iris|ja(t|v)a|jbro|jemu|jigs|kddi|keji|kgt( |\/)|klon|kpt |kwc\-|kyo(c|k)|le(no|xi)|lg( g|\/(k|l|u)|50|54|\-[a-w])|libw|lynx|m1\-w|m3ga|m50\/|ma(te|ui|xo)|mc(01|21|ca)|m\-cr|me(rc|ri)|mi(o8|oa|ts)|mmef|mo(01|02|bi|de|do|t(\-| |o|v)|zz)|mt(50|p1|v )|mwbp|mywa|n10[0-2]|n20[2-3]|n30(0|2)|n50(0|2|5)|n7(0(0|1)|10)|ne((c|m)\-|on|tf|wf|wg|wt)|nok(6|i)|nzph|o2im|op(ti|wv)|oran|owg1|p800|pan(a|d|t)|pdxg|pg(13|\-([1-8]|c))|phil|pire|pl(ay|uc)|pn\-2|po(ck|rt|se)|prox|psio|pt\-g|qa\-a|qc(07|12|21|32|60|\-[2-7]|i\-)|qtek|r380|r600|raks|rim9|ro(ve|zo)|s55\/|sa(ge|ma|mm|ms|ny|va)|sc(01|h\-|oo|p\-)|sdk\/|se(c(\-|0|1)|47|mc|nd|ri)|sgh\-|shar|sie(\-|m)|sk\-0|sl(45|id)|sm(al|ar|b3|it|t5)|so(ft|ny)|sp(01|h\-|v\-|v )|sy(01|mb)|t2(18|50)|t6(00|10|18)|ta(gt|lk)|tcl\-|tdg\-|tel(i|m)|tim\-|t\-mo|to(pl|sh)|ts(70|m\-|m3|m5)|tx\-9|up(\.b|g1|si)|utst|v400|v750|veri|vi(rg|te)|vk(40|5[0-3]|\-v)|vm40|voda|vulc|vx(52|53|60|61|70|80|81|83|85|98)|w3c(\-| )|webc|whit|wi(g |nc|nw)|wmlb|wonu|x700|yas\-|your|zeto|zte\-/i.test(ua.substr(0,4));
}

/*function handleCenterNavArrow()
{
	$($("input[type=checkbox]:checked ~ ul > li").each(function(){
		if($(this).height() > 20)
	});
	if($("input[type=checkbox]:checked ~ ul > li:nth-child(1)").height() < 
	   $("input[type=checkbox]:checked ~ ul > li:nth-child(2)").height()) {
	   $("input[type=checkbox]:checked ~ ul > li:nth-child(2)").addClass("center");
   } else {
	   $("input[type=checkbox]:checked ~ ul > li:nth-child(2)").removeClass("center");
   }
}*/

var mousewheelevt = (/Firefox/i.test(navigator.userAgent)) ? "DOMMouseScroll" : "mousewheel";
$(function(){
	var isMobile = checkMobile();
	
	$.post("admin/execute.php", { cmd:"insert", lang:$("#langSwitcher > a").attr("data-lang-from") });
	var fancy, navTop = $('.main-nav').length && $('.main-nav').offset().top, isFixed = 0, lastScrollTop = 0;
	fancy = $(".fancybox").fancybox({
		openEffect	: 'none',
		closeEffect	: 'none'
	});
	
	/*$(window).on("resize", function(e){
		if(parseInt(viewport().width) < 510) // == @nav-braeakpoint
			handleCenterNavArrow();
	});*/
	
	$(window).on(mousewheelevt + " scroll", function(e){
		var scrollUp = isScrollingUp();
		if(!isMobile)
		{
			/* var evt = window.event || e;
			evt = evt.originalEvent ? evt.originalEvent : evt;
			var delta = evt.detail ? evt.detail*(-40) : evt.wheelDelta;
			var scrollUp = delta > 0 ? true : false;*/
			processScroll(scrollUp);
		}
		else
		{
			var $nav = $('.main-nav');
			if (scrollUp) {
				$nav.addClass('main-nav-fixe');
				// .main-nav-fixed{position:fixed;top:0;width:100%;max-width:960px;opacity:0.9;z-index:999;}
			} else {
				$nav.removeClass('main-nav-fixed');
			}
		}
	});
	
	$(document).on("click submit change keyup", function(e){
		var eventType = e.type;
		var target = $(e.target);
		
		if(!target.is(".navigation a") && !target.is(".navigation li") && !target.is("#langSwitcher a") &&
		   !target.is("#gas_trigger") && !target.is("input#invia") && !target.is("#message") &&
		   !target.is("#mailMessage") /* && !target.is(".projectTitle a")*/)
			return;
		
		if(eventType == "click")
		{
			e.stopPropagation();
			e.preventDefault();
			
			/*if(target.is(".projectTitle a")) {
				$.post("admin/execute.php", { cmd:"increment-counter", param:target.attr("href").substring(11) }, function(data){
					var obj = JSON.parse(data);
					console.log(obj);
				});
			}*/
			
			if(target.is(".navigation a"))
			{
				var li = target.closest("li");
				var waypoint = li.find("a");
				goToByScroll(waypoint.attr("href"));
				$(".navigation li.active").removeClass("active");
				li.addClass("active");
			}
			else if(target.is(".navigation li"))
			{
				var li = target;
				var waypoint = li.find("a");
				goToByScroll(waypoint.attr("href"));
				$(".navigation li.active").removeClass("active");
				li.addClass("active");
			}
			else if(target.is("#langSwitcher a"))
			{
				var url, header = {}, to = $(e.target).attr("data-lang-to");
				header.nav = {};
				if(to == "en") {
					url = "english.php";
					header.subTitle = "Personal Web Site";
					header.nav[0] = "Resume";
					header.nav[1] = "Work Experience";
					header.nav[2] = "Personal-Academic Projects";
					header.nav[3] = "Contact Me";
					header.classToAdd = "italian";
					header.classToRemove = "english";
					header.dataLangTo = "it";
					header.dataLangFrom = "en";
				} else if(to == "it") {
					url = "italian.php";
					header.subTitle = "Sito Web Personale";
					header.nav[0] = "Curriculum Vitae";
					header.nav[1] = "Esperienze lavorative";
					header.nav[2] = "Progetti Personali-Accademici";
					header.nav[3] = "Contatti";
					header.classToAdd = "english";
					header.classToRemove = "italian";
					header.dataLangTo = "en";
					header.dataLangFrom = "it";
				}
				
				$.get(url, function(data){
					$.post("admin/execute.php", { cmd:"insert", lang:header.dataLangFrom });
					$(".innerContainer").fadeOut("fast", function(){
						$(".innerContainer").html(data);
						$(".innerContainer").fadeIn("fast", function(){
							$(".siteSubtitle").html(header.subTitle);
							$(".navigation a").each(function(i){
								$(this).html(header.nav[i]);
							});
							var langSwitcher = $("#langSwitcher a");
							langSwitcher.removeClass(header.classToRemove);
							langSwitcher.addClass(header.classToAdd);
							langSwitcher.attr("data-lang-to", header.dataLangTo);
							langSwitcher.attr("data-lang-from", header.dataLangFrom);
							fancy = $(".fancybox").fancybox({
								openEffect	: 'none',
								closeEffect	: 'none'
							});
							processScroll();
						});
					});
				});
			}
			else if(target.is("#gas_trigger"))
			{
				$.fancybox.open(fancy);
				return false;
			}
			else if(target.is("input#invia"))
			{
				var	message = $("#message");
				var mailMessage = $("#mailMessage");
				var ajaxLoader = $("#ajax-loader");
				if(message.val().trim() == ""){
					message.addClass("error");
					return;
				}
				message.removeClass("error");
				mailMessage.slideUp("fast");
				mailMessage.removeClass("success");
				mailMessage.removeClass("failure");
				ajaxLoader.addClass("loader");
				$.post("sendMail.php", { message:$("#message").val() }, function(data){
					ajaxLoader.removeClass("loader");
					var obj = JSON.parse(data);
					mailMessage.html(obj.message);
					if(obj.ok == "true") {
						mailMessage.addClass("success");
					} else {
						mailMessage.addClass("failure");
						message.addClass("error");
					}
					mailMessage.slideDown("fast");
				});
			}
			else if(target.is("#mailMessage"))
			{
				target.slideUp("fast");
			}
		}
		else if(eventType == "change" && target.is("#menu"))
		{
			if($(this).is(":checked"))
				$(".main-nav").addClass("opacity-small-nav");
			else
				$(".main-nav").removeClass("opacity-small-nav");
		}
		else if(eventType == "keyup")
		{
			$("#message").removeClass("error");
		}
	});
	
	function processScroll(scrollUp)
	{
		var i,
			scrollTop = $(window).scrollTop(),
			$nav = $('.main-nav');
			
		if (scrollTop >= navTop && !isFixed) {
			isFixed = 1;
			$nav.addClass('main-nav-fixed');
		} else if (scrollTop <= navTop && isFixed) {
			isFixed = 0;
			$nav.removeClass('main-nav-fixed');
		}
		
		var map = new Object();
		$(".waypoint").each(function(i){
			var el = $(this);
			if(isScrolledIntoView(el)) {
				map[el.attr("data-waypoint")] = el.offset().top;
			}
		});
		
		// Se scrollo verso il basso seleziono il waypoint piu' in basso, altrimenti quello piu' in alto
		var idToSelect = "";
		if(!jQuery.isEmptyObject(map))
		{			
			if(scrollUp)
			{
				var topMost = Number.MAX_VALUE;
				for(var id in map)
				{
					if(map[id] < topMost)
					{
						topMost = map[id];
						idToSelect = id;
					}
				}
			}
			else
			{
			
				var downMost = Number.MIN_VALUE;
				for(var id in map)
				{
					if(map[id] > downMost)
					{
						topMost = map[id];
						idToSelect = id;
					}
				}
			}
			$(".navigation li.active").removeClass("active");
			$(".navigation li a[href='#" + idToSelect + "']").closest("li").addClass("active");
		}
	}
	
	function isScrollingUp()
	{
		var currentScrollTop = $(document).scrollTop();
		var scrollingUp = false;
		
		if(currentScrollTop < lastScrollTop)
			scrollingUp = true;
		lastScrollTop = currentScrollTop;
		
		return scrollingUp;
	}
});