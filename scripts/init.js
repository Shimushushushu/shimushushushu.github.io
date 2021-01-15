$ = mdui.$;

$('#theme-switch').on('click', function(e) {
  if ($('body').hasClass('mdui-theme-layout-dark')) {
    $('body').removeClass('mdui-theme-layout-dark');
    $('body').addClass('mdui-theme-layout-light');
    document.cookie = "dark=0; path=/";
  } else {
    $('body').removeClass('mdui-theme-layout-light');
    $('body').addClass('mdui-theme-layout-dark');
    document.cookie = "dark=1; path=/";
  }
});

function readcookie() {
  tmp = document.cookie;
  if (tmp != "") {
    tmp = tmp.split(';')[0].split('=')[1];
    if (tmp == '1') {
      $('body').removeClass('mdui-theme-layout-light');
      $('body').addClass('mdui-theme-layout-dark');
    } else {
      $('body').removeClass('mdui-theme-layout-dark');
      $('body').addClass('mdui-theme-layout-light');
    }
  }
}