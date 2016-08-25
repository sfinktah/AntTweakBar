define("hbs!common/templates/common/advert",["hbs","common/hbs/handlebars"],function(e,t){var a=t.template(function(e,t,a){this.compilerInfo=[4,">= 1.0.0"],a=this.merge(a,e.helpers);var s,o,n="",r="function",l=this.escapeExpression;return n+='<li>\r\n    <a href="#" class="',(o=a.ClickClass)?s=o.call(t,{hash:{}}):(o=t&&t.ClickClass,s=typeof o===r?o.call(t,{hash:{}}):o),n+=l(s)+'" data-action="',(o=a.ActionData)?s=o.call(t,{hash:{}}):(o=t&&t.ActionData,s=typeof o===r?o.call(t,{hash:{}}):o),n+=l(s)+'" data-title="',(o=a.LinkText)?s=o.call(t,{hash:{}}):(o=t&&t.LinkText,s=typeof o===r?o.call(t,{hash:{}}):o),n+=l(s)+'">\r\n        <img src="',(o=a.ImageUrl)?s=o.call(t,{hash:{}}):(o=t&&t.ImageUrl,s=typeof o===r?o.call(t,{hash:{}}):o),n+=l(s)+'" />\r\n        <div>\r\n            <h2>',(o=a.Title)?s=o.call(t,{hash:{}}):(o=t&&t.Title,s=typeof o===r?o.call(t,{hash:{}}):o),n+=l(s)+"</h2>\r\n            <h3>",(o=a.Body)?s=o.call(t,{hash:{}}):(o=t&&t.Body,s=typeof o===r?o.call(t,{hash:{}}):o),n+=l(s)+"</h3>\r\n        </div>\r\n    </a>\r\n</li>"});return a}),define("common/games/gta5",["jquery","common/xhr","utils","underscore","common/localize","setup.environment","setup.environment.utils","common/datafunctions","common/games/gamecommon","common/modals/modal-views","common/store/store-views","am","hbs!common/templates/common/advert"],function(e,t,a,s,o,n,r,l,i,c,d,m,g){function u(){var t=new e.Deferred;if(a.isStoreV2Enabled()){var s=function(){t.resolve(!0)},o=function(){t.resolve(!1)},n=function(){t.reject()},r={ofAgeCallbkFn:s,underAgeCallbkFn:o,errorCallbkFn:n};a.pubsub.trigger("setupEnvUtils:checkStoreAgeAppropriate",r)}return t.promise()}function h(e){switch(e){case x.FIND_CREW:case x.GO_TO_URL:case x.NONE:return!1;case x.STORE_WITH_SEARCH:case x.STORE_WITH_CATEGORY:default:return!0}}function f(e){switch(e){case x.STORE_WITH_SEARCH:return"clickEvent storeWithSearch";case x.STORE_WITH_CATEGORY:return"clickEvent storeWithCategory";case x.FIND_CREW:return"clickEvent goToFindCrew";case x.GO_TO_URL:return"clickEvent goToUrl";case x.NONE:return"clickEvent none";default:return"clickEvent storeDefault"}}function v(t){t.off("click","a.storeWithSearch").on("click","a.storeWithSearch",function(){m.trackEvent(m.category.advert,m.action.advert_storeWithSearch);var t=e("#storeButton");t.removeData(),t.data("productId",e(this).data("action")),d.showCommerceV2()}),t.off("click","a.storeWithCategory").on("click","a.storeWithCategory",function(){m.trackEvent(m.category.advert,m.action.advert_storeWithCategory);var t=e("#storeButton");t.removeData(),t.data("category",e(this).data("action")),d.showCommerceV2()}),t.off("click","a.storeDefault").on("click","a.storeDefault",function(){m.trackEvent(m.category.advert,m.action.advert_storeDefault),d.showCommerceV2()}),t.off("click","a.goToFindCrew").on("click","a.goToFindCrew",function(){m.trackEvent(m.category.advert,m.action.advert_findCrew),n.showCrewSearch(e(this).data("action"))}),t.off("click","a.goToUrl").on("click","a.goToUrl",function(){m.trackEvent(m.category.advert,m.action.advert_goToUrl);var t=e(this).data("action"),a=e(this).data("title");if(r.isUrlWhitelisted(t)){var s=o.getText("LinkConfirm","HomeSignin").replace("[linkText]",a);c.ShowConfirmModal(s,"","setupEnvUtils:launchExternalBrowserForUrl",t)}})}function p(){if(0===E){var t=e("#featuredPromotion"),a=1e4;E=setInterval(function(){if(!t.find("li").length||1===t.find("li").length||!r.isActivePanel("#homefeedPanel"))return void b();var e=t.find("li:visible"),a=e.next();0===a.length&&(a=t.find("li:eq(0)")),e.fadeOut(1e3),a.fadeIn(1e3)},a)}}function k(t){t.find("img").on("error",function(){T(e(this))})}function T(t){t.parents("li").remove();var a=e("#featuredPromotion").find("li");a.length?1===a.length?(a.show(),b()):(a.first().show(),b(),p()):(e("#featuredPromotion").hide(),b())}function b(){clearInterval(E),E=0}function C(){S.statsCache={}}function y(){C(),globals.adsLoaded=!1,e("#featuredPromotion").hide()}var S={};S.statsCache={};var E=0;S.GetLauncherConfig=function(){return{images:["images/gta5/launcher/img1.jpg","images/gta5/launcher/img2.jpg","images/gta5/launcher/img3.jpg","images/gta5/launcher/img4.jpg","images/gta5/launcher/img5.jpg","images/gta5/launcher/img6.jpg","images/gta5/launcher/img7.jpg","images/gta5/launcher/img8.jpg","images/gta5/launcher/img9.jpg","images/gta5/launcher/img10.jpg","images/gta5/launcher/img11.jpg","images/gta5/launcher/img12.jpg","images/gta5/launcher/img13.jpg","images/gta5/launcher/img14.jpg","images/gta5/launcher/img15.jpg","images/gta5/launcher/img16.jpg","images/gta5/launcher/img17.jpg","images/gta5/launcher/img18.jpg"],imgInterval:5e3}},S.LoadHomeScreen=function(){i.LoadHomeScreen(),globals.advertsEnabled&&(globals.adsLoaded?p():S.loadAds())},S.displayGameLogo=function(){e("#launcherLogo").length||e("#launcherStyleContent").prepend(e("<img>",{id:"launcherLogo",src:globals.baseUrl+"images/gta5/gtav_logo.png",alt:"Grand Theft Auto V"}))},S.loadAds=function(){var e=r.getCredsAsync();e.done(function(e){if(e.Ticket){var s='{ "playerTicket":"'+e.Ticket+'","env":"'+globals.rosEnvironment+'","title":"'+globals.titleName+'","version":'+globals.RosTitleVersion+'" }',o=r.GetAuthHeaderAsync(e.Ticket);o.done(function(e){var o=a.getApiUrl("api/"+globals.titleName+"/GetAdverts"),n=t.ajax({type:"POST",data:s,headers:e,async:!0,url:o,dataType:"json",silentFail:!0});n.done(function(e,t){"success"===t&&(S.renderAdverts(e),globals.adsLoaded=!0)})})}})},S.renderAdverts=function(t){var s=t.totalAdCount,o=t.adverts;if(0!==s&&null!==o&&"undefined"!=typeof o&&o.length){var n=e("#featuredPromotion"),r="",l=u();l.done(function(t){for(var l=0;l<o.length;l++){var i=o[l],c=i.languages[globals.titlelang];"undefined"==typeof c&&(c=i.languages["en-US"]);var d;d="undefined"!=typeof c.actionData&&null!==c.actionData&&""!==c.actionData?c.actionData:i.actionData;var m="";i.clickAction===x.GO_TO_URL&&(m="undefined"!=typeof c.linkText&&null!==c.linkText&&""!==c.linkText?c.linkText:d);var u=f(i.clickAction),T={ImageUrl:i.imageUrl,Title:c.title,Body:c.body,ClickClass:u,ActionData:d,LinkText:m};(t||!h(i.clickAction))&&(r+=a.renderCompiledTemplate(g,T))}n.find("ul").html(r),r.length&&(k(n),v(n),n.find("li:gt(0)").hide(),s>0&&o.length>1&&p(n),e("#featuredPromotion").fadeIn(1e3))})}};var x={STORE_WITH_SEARCH:0,STORE_WITH_CATEGORY:1,FIND_CREW:2,GO_TO_URL:3,NONE:4};S.LoadPlayerStats=function(s,o){a.WriteDebugLog("LoadPlayerStats","compareRockstarId",s);var n=o?"/OnlineStats":"/Stats",r=globals.baseAssetUrl+"xml/"+globals.titleName+n+globals.titlelang+".xml";t.ajax({type:"GET",async:!1,url:r,dataType:"xml",misc:{compareId:s,onlineStats:o},success:function(t){var a=[];e(t).find("id").each(function(){a.push(e(this).text())});var s=this.misc.compareId,o=this.misc.onlineStats;S.GetPlayerStatsJS(t,a.join(","),globals.account.RockstarId,s,o),t=null,a=null},complete:function(e,t){a.ToggleCompareFriendElements(t)}})};var A=function(t,a){r.resetScrollbar("#statsContainer"),r.removeLoading("statsPanel"),e("#awardsLink2").removeClass("disabled"),a?e("#statsLink2").removeClass("disabled"):e("#onlineStatsLink").removeClass("disabled"),"success"!==t&&e("#comparedFriend").hide()},I=function(t,a){return"success"===a&&"true"===e(t).find("Status").text()},P=function(t,s,n,l){var i,c,d=2;null!==s&&void 0!==s&&(e("#statsPanel").addClass("compared"),d=3);var m=e(t).find("StatsRecord");m.each(function(){e(this).find("GamerHandle").text()=="SC "+n?i=e(this).find("Values").text().split(","):c=e(this).find("Values").text().split(",")});var g=103,u=[],h="";e(l).find("id").each(function(t){var s=e(this),n="Conditional"==s.parent().attr("Comment"),r=s.parent().parent().find("header_name");r.length>0&&r.text()!=h&&(h=r.text(),u.length>0&&u.push("</tbody>"),u.push("<tbody id='"+h+"' class='statgroup' tabindex='"+g+"'><tr class='levelName statGroupHeader'><td colspan='"+d+"' class='levelName clearfix "+s.parent().parent()[0].nodeName+"'><div class='headerName'>"+h+"</div><span></span></td></tr>"),g++);var l=s.text();if(""!=l.length){var m=void 0!=i&&void 0!=i[t]?i[t]:"-";if(!n||n&&"0"!=m){var f="",v=s.parent().find("type");switch(v.text().toLowerCase()){case"duration":"-"!=m&&(m=a.msToTime(m)),e("#statsPanel").hasClass("compared")&&(f="<td class='col3'>"+(void 0!=c&&void 0!=c[t]&&"-"!=c[t]?a.msToTime(c[t]):"-")+"</td>");break;case"percentage":"-"!=m&&(m=a.formatPercent(m)),e("#statsPanel").hasClass("compared")&&(f="<td class='col3'>"+(void 0!=c&&void 0!=c[t]&&"-"!=c[t]?a.formatPercent(c[t]):"-")+"</td>");break;case"meterstomiles":"-"!=m&&(m=a.metersToMiles(m)),e("#statsPanel").hasClass("compared")&&(f="<td class='col3'>"+(void 0!=c&&void 0!=c[t]&&"-"!=c[t]?a.metersToMiles(c[t]):"-")+"</td>");break;case"meterstofeet":"-"!=m&&(m=a.metersToFeet(m)),e("#statsPanel").hasClass("compared")&&(f="<td class='col3'>"+(void 0!=c&&void 0!=c[t]&&"-"!=c[t]?a.metersToFeet(c[t]):"-")+"</td>");break;case"kphtomph":"-"!=m&&(m=a.kphToMph(m)),e("#statsPanel").hasClass("compared")&&(f="<td class='col3'>"+(void 0!=c&&void 0!=c[t]&&"-"!=c[t]?a.kphToMph(c[t]):"-")+"</td>");break;case"money":"-"!=m&&(m=a.formatMoney(m)),e("#statsPanel").hasClass("compared")&&(f="<td class='col3'>"+(void 0!=c&&void 0!=c[t]&&"-"!=c[t]?a.formatMoney(c[t]):"-")+"</td>");break;case"boolean":"-"!=m&&(m=0==m?o.getText("No","Social"):o.getText("Yes","Social")),e("#statsPanel").hasClass("compared")&&(f="<td class='col3'>"+(void 0!=c&&void 0!=c[t]&&"-"!=c[t]?0==c[t]?o.getText("No","Social"):o.getText("Yes","Social"):"-")+"</td>");break;case"meters":"-"!=m&&(m=a.formatMeters(m)),e("#statsPanel").hasClass("compared")&&(f="<td class='col3'>"+(void 0!=c&&void 0!=c[t]&&"-"!=c[t]?a.formatMeters(c[t]):"-")+"</td>");break;case"decimal":"-"!=m&&(m=a.decimal(m)),e("#statsPanel").hasClass("compared")&&(f="<td class='col3'>"+(void 0!=c&&void 0!=c[t]&&"-"!=c[t]?a.decimal(c[t]):"-")+"</td>");break;default:e("#statsPanel").hasClass("compared")&&(f="<td class='col3'>"+(void 0!=c&&void 0!=c[t]?c[t]:"-")+"</td>")}var p=s.parent().find("name");p.length>0&&u.push("<tr style='display:none' id='statId"+l+"' class='"+s.parent().parent()[0].nodeName+"'><td class='col1'>"+p.text()+"</td>"+f+"<td class='col2'>"+m+"</td></tr>")}}}),u.length>0&&u.push("</tbody>"),e("#statstable").html(u.join("")),e("#statstable .statgroup").keydown(function(t){13==t.which&&e(this).click()}),e("#statstable .statgroup").click(function(){var t=e(this);return t.find("tr.levelName.statGroupHeader").toggleClass("open"),t.children(":not(.statGroupHeader)").toggle(),r.resetScrollbar("#statsPanel"),!1}),u=null,i=null,c=null},L=function(t,s,o,n,r){"success"==s&&("true"==e(t).find("Status").text()?P(t,o,n,r):a.WriteDebugLog("GetPlayerStatsJS","Status error",e(t).find("Error").text(),a.logType.error,a.logController[globals.titleName])),e("#checkName").fadeOut()};return S.GetPlayerStatsJS=function(s,n,l,i,d){var m=d?"getgtavonlineplayerstats":"getplayerstatsws",g=a.getApiUrl("api/"+globals.titleName+"/"+m),u=[globals.titleName,m,l,i?i:-1].join(":");if(S.statsCache[u]){var h=S.statsCache[u],f="success";return L(h,f,i,l,s),void A(f,d)}var v=r.getCredsAsync();v.done(function(m){if(m&&m.Ticket){var h='{ "env":"'+globals.rosEnvironment+'","title":"'+globals.titleName+'","version":'+globals.RosTitleVersion+', "wsURL":"'+globals.wsURL+'","platformId":'+globals.platformId+',"statIds":"'+n+'","rockstarIds":"'+l+(void 0!=i?","+i:"")+'" }';a.WriteDebugLog("GetPlayerStatsJS","options",h);var f=r.GetAuthHeaderAsync(m.Ticket);f.done(function(e){t.ajax({type:"POST",url:g,headers:e,data:h,contentType:"application/json; charset=utf-8",dataType:"xml",cache:!1,services:["stats"],fatalError:c.ShowErrorModal,requestDescription:"GetPlayerStats",misc:{xmlData:s,playerRockstarId:l,compareRockstarId:i,onlineStats:d},success:function(e,t){I(e,t)&&(S.statsCache[u]=e);var a=this.misc.xmlData,s=this.misc.playerRockstarId,o=this.misc.compareRockstarId;L(e,t,o,s,a)},complete:function(e,t){s=null,n=null,l=null,i=null;var a=this.misc.onlineStats;A(t,a)}})})}else r.removeLoading("statsPanel"),e("#statstable").html("<tr><td>"+o.getError("InternetAccess")+"</td></tr>")})},a.pubsub.on("section:stats:loaded",C),a.pubsub.on("signout:completed",y),S});