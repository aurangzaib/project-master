    (function () {

      var scr_lnr = document.createElement("SCRIPT");
      scr_lnr.src = 'https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js';
      scr_lnr.type = 'text/javascript';

      var css_lnr = document.createElement("LINK");
      css_lnr.href = "https://rawgit.com/aurangzaib/master-project/master/listnride.css";
      css_lnr.rel = "stylesheet";

      var css_mdl = document.createElement("LINK");
      css_mdl.href = "https://code.getmdl.io/1.2.1/material.blue_grey-blue.min.css";
      css_mdl.rel = "stylesheet";

      var fonts_mdl = document.createElement("LINK");
      fonts_mdl.href = "https://fonts.googleapis.com/icon?family=Material+Icons";
      fonts_mdl.rel = "stylesheet";

      var head = document.getElementsByTagName("head")[0];
      head.appendChild(fonts_mdl);
      head.appendChild(css_mdl);
      head.appendChild(css_lnr);
      head.appendChild(scr_lnr);

      var checkReady = function (callback) {
        if (window.jQuery) {
          callback(jQuery);
        } else {
          window.setTimeout(function () {
            checkReady(callback);
          }, 100);
        }
      };

      // Start polling...
      checkReady(function ($) {
        $(function () {
          $.get("https://api.listnride.com/v2/featured", function (a) {
            var id_lnr = $("#listnride");
            id_lnr.html('<div class="mdl-grid" id="lnr-grid"></div>');
            var grid = $("#lnr-grid");
            a.forEach(function (a) {
              grid.append(
                '<div class="mdl-cell mdl-cell--4-col mdl-cell--middle"><div class="lnr-card-wide mdl-card mdl-shadow--2dp"><div class="mdl-card__media"><img src="' +
                a.image_file_1.image_file_1.small.url +
                '" width="100%" height="80%"></div><div class="mdl-card__supporting-text">' + a.brand +
                ", " + a.category + "</div></div></div>");
            });
          });
        });
      });
    })();
