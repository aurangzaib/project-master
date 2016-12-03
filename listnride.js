// this script has nothing to do w/ master project. its just testing script for my company work.

// this script will render the bikes of listnride in any site.
// the site has to include this js file in their html.
// and add `<div>

$(function () {
  $.get("https://api.listnride.com/v2/featured", function (a) {
    $.get("https://rawgit.com/aurangzaib/master-project/master/listnride.css", function (listnRideCssData) {
      $.get("https://fonts.googleapis.com/icon?family=Material+Icons", function (materialIcons) {
        $.get("https://code.getmdl.io/1.2.1/material.blue_grey-blue.min.css", function (a) {
          a.forEach(function (a) {
            $("#listnride").append(
              '<div class="mdl-cell mdl-cell--4-col mdl-cell--middle"><div class="lnr-card-wide mdl-card mdl-shadow--2dp"><div class="mdl-card__media"><img src="' +
              a.image_file_1.image_file_1.small.url +
              '" width="100%" height="80%"></div><div class="mdl-card__supporting-text">' + a.brand + ", " +
              a.category + "</div></div></div>")
          });
        });
      });
    });
  });
});
