!function () {
  var a = document.createElement("SCRIPT");
  a.src = "https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js", a.type = "text/javascript";
  var b = document.createElement("LINK");
  b.href = "https://rawgit.com/aurangzaib/master-project/master/listnride.css", b.rel = "stylesheet";
  var c = document.createElement("LINK");
  c.href = "https://code.getmdl.io/1.2.1/material.blue_grey-blue.min.css", c.rel = "stylesheet";
  var d = document.createElement("LINK");
  d.href = "https://fonts.googleapis.com/icon?family=Material+Icons", d.rel = "stylesheet";
  var e = document.getElementsByTagName("head")[0];
  e.appendChild(d), e.appendChild(c), e.appendChild(b), e.appendChild(a);
  var f = function (a) {
    window.jQuery ? a(jQuery) : window.setTimeout(function () {
      f(a)
    }, 100)
  };
  f(function (a) {
    a(function () {
      a.get("https://api.listnride.com/v2/featured", function (b) {
        var c = a("#listnride");
        b.forEach(function (a) {
          c.append('<div class="mdl-cell mdl-cell--4-col mdl-cell--middle"><div class="lnr-card-wide mdl-card mdl-shadow--2dp"><div class="mdl-card__media"><img src="' + a.image_file_1.image_file_1.small.url + '" width="100%" height="80%"></div><div class="mdl-card__supporting-text">' + a.brand + ", " + a.category + "</div></div></div>")
        })
      })
    })
  })
}();
