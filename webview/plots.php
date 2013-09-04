<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">

<html>
<head>
  <title>RHIC Polarimetry Results: Summary Plots</title>
  <meta HTTP-EQUIV="PRAGMA" CONTENT="NO-CACHE">
  <link REL="STYLESHEET" TYPE="text/css" HREF="main.css">
  <link REL="STYLESHEET" TYPE="text/css" HREF="wiki.css">
  <link REL="STYLESHEET" TYPE="text/css" HREF="wiki_monobook_main.css">
  <script language="javascript" src="main.js"></script>
  <script language="javascript" src="toc.js"></script>
</head>

<body bgcolor="#ffffff">


<?php
include("head.html");

include_once("config.php");
include_once("PlotHelper.php");

//include_once("../rundb/DbFillPolar.php");
//$dbFillPolar = new DbFillPolar();
//$nFills = $dbFillPolar->CountEntries();

$dir = "results/run11_pp_transverse/";
//$dir = "results/run11_mc_Wp2enu_embed/";
$gP = new PlotHelper($dir);

//$flattopEnergy = sprintf("%03d", 24);
//$flattopEnergy = sprintf("%03d", 100);
//$ai['fBeamEnergies'] = $_GET['es'];
$flattopEnergy = sprintf("%03d", 255);

?>

<!-- Main text starts here-->

<h1>Summary Plots</h1>

<div class="count" id="generated-toc"></div>

<hr>

<div id="main_text_body">
<!-- {{{ -->



<h1 id="hjet" class="count">W Boson Events</h1>
<!-- {{{ -->
<div id="div:wbos" class="section">


<h2 id="hjet_an" class="count">Asymmetry</h2>
<!-- {{{ -->
<div id="div:wbos_asym" class="section">


<p>
<table class="simple00 cntr">
<tr>
   <? foreach ($RHIC_BEAMS as $indx => $beam): ?>
   <td><?=$gP->img("asym_w_plus/hWBosonAsymAmpVsEta_$beam", false, 600)?>
   <div class='thumbcaption_cm'><span class=bluPol><?=$beam?></span></div>
   <? endforeach; ?>

<tr>
<td colspan=2>
<div class='thumbcaption'>Asymmetry</div>

<tr>
   <? foreach ($RHIC_BEAMS as $indx => $beam): ?>
   <td><?=$gP->img("asym_w_plus/hWBosonAsymAmpVsPt_$beam", false, 600)?>
   <div class='thumbcaption_cm'><span class=bluPol><?=$beam?></span></div>
   <? endforeach; ?>

<tr>
<td colspan=2>
<div class='thumbcaption'>Asymmetry</div>

</table>


</div>
<!-- }}} -->


</div>
<!-- }}} -->




<!-- Main text ends here-->
</div>
<!-- }}} -->



<?php
include("bottom.html");
?>

</body>
</html>