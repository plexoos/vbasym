<html>
<head>
  <title>p-Carbon Measurement <?=$gRunId?>: Analysis Results</title>
  <link REL="STYLESHEET" TYPE="text/css" HREF="../main.css">
  <link REL="STYLESHEET" TYPE="text/css" HREF="../wiki.css">
  <link REL="STYLESHEET" TYPE="text/css" HREF="../wiki_monobook_main.css">
  <script language="javascript" src="../main.js"></script>
  <script language="javascript" src="../toc.js"></script>
</head>

<body bgcolor="#ffffff">

<?php

include_once("config.php");

$event_display_dir = VBASYM_RESULTS_DIR."run11_pp_transverse_ready/event_display/";
$dir_handle = opendir($event_display_dir);

$run_events = array();

while (false !== ($file = readdir($dir_handle))) {
    //echo "$file - ";
    if (preg_match("/^r([0-9]{8})_([0-9]{7})$/", $file, $matches)) {
       //echo "matched {$matches[1]} {$matches[2]}<br>\n";
       $run_events[] = array($matches[1], $matches[2]);
       
    } else {
       //echo "did not match<br>\n";
    }
}

$totalEvents = count($run_events);
print("events: $totalEvents<br>\n");
$remain = $totalEvents%20;

$nPages = $remain > 0 ? intval($totalEvents/20) + 1 : $totalEvents/20;

for ($ipage=1; $ipage<=$nPages; $ipage+=1)
{
   if (intval($_GET['p']) == $ipage)
      print("$ipage\n");
   else
      print("<a href='?p=$ipage'>$ipage</a>\n");
}

?>

<p>

<table border=1 class="simple00 cntr" cellspacing=15 align=center>

<?php

$i_event = 0;
$first_event = (intval($_GET['p'])-1)*20 + 1;
//$first_event = 1;

foreach ($run_events as $run_event)
{
   $i_event += 1;

   if ($i_event < $first_event) continue;
   if ($i_event >= $first_event+20) break;
   if (($i_event-$first_event) % 4 == 0) {
      print "<tr class=align_cm>\n";
   }

   $imgHref = VBASYM_RESULTS_WWW."/run11_pp_transverse_ready/event_display/r{$run_event[0]}_{$run_event[1]}/c_hBTowEnergy.png";
   $html    = "<a href=\"$imgHref\"><img width=400 src=\"$imgHref\" class='thumbimage'></a>";

   print "<td>$html<br>\n";
}

?>

</table>

</body>
</html>
