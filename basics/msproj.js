fs = require('fs')
fs.readFile('/Users/aurangzaib/Documents/Projects/master-project/master-project//Meeting-7/teach-2-1-maxima.txt', 'utf8', function (err, data) {
  if (err) {
    return console.log(err);
  }
  //console.log("length: " , data);
  
  // string to array
  var stringArray = data.split('\n');
  var outputArray = [];
  
  for (var loopVar = 1; loopVar < stringArray.length; loopVar++) {
    // get index of *
    var i = stringArray[loopVar].indexOf("*");
    // push selected value in new array
    (i > 0) && outputArray.push(stringArray[loopVar].slice(0, i));
  }

  // save in a file
  fs.writeFile("/Users/aurangzaib/Documents/Projects/master-project/master-project//Meeting-7/teach-2-1-maxima-parsed.txt", outputArray, function (err) {
    if (err) {
      return console.log(err);
    }
    console.log("The file was saved!");
  });

});