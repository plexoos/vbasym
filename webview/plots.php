<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">

<html>
<head>
  <title>Vector Boson Asymmetry Measurement. Plots</title>
  <meta HTTP-EQUIV="PRAGMA" CONTENT="NO-CACHE">
  <link REL="STYLESHEET" TYPE="text/css" HREF="css/main.css">
  <link REL="STYLESHEET" TYPE="text/css" HREF="css/wiki.css">
  <link REL="STYLESHEET" TYPE="text/css" HREF="css/wiki_monobook_main.css">
  <script language="javascript" src="js/main.js"></script>
  <script language="javascript" src="js/toc.js"></script>
</head>

<body bgcolor="#ffffff">


<?php
include("head.html");

include_once("config.php");
include_once("PlotHelper.php");

$ana = isset($_GET['ana']) ? $_GET['ana'] : "run11_pp_transverse";
$asym_type = isset($_GET['at']) ? $_GET['at'] : "";
$asym_type = !empty($asym_type) ? "_$asym_type" : "";

$dir = "results/$ana/";
$gP = new PlotHelper($dir);

?>

<!-- Main text starts here-->

<h1>Plots</h1>

<div class="count" id="generated-toc"></div>

<hr>

<div id="main_text_body">
<!-- {{{ -->


<h1 id="wbos_asym" class="count">W Boson Events Asymmetry</h1>
<!-- {{{ -->
<div id="div:wbos_asym" class="section">


<h2 id="wbos_asym_vs_eta" class="count">W boson asymmetry vs. pseudorapidity eta</h2>
<!-- {{{ -->
<div id="div:wbos_asym_vs_eta" class="section">


<? foreach ($VECBOS_TYPE_SUFFIXES as $vecbos_type_idx => $vecbos_type_sfx): ?>

<h3 id="wbos_asym_vs_eta_<?=$vecbos_type_sfx?>" class="count"><?=$VECBOS_TYPE_HUMAN_DESCR[$vecbos_type_idx]?></h3>
<!-- {{{ -->
<div id="div:wbos_asym_vs_eta_<?=$vecbos_type_sfx?>" class="section">

<p>
<table class="simple00 cntr">

<?
   $row1 = "<tr>\n";
   $row2 = "<tr>\n";

   foreach ($RHIC_BEAM_SUFFIXES as $beam_index => $beam_sfx):

      $row1 .= "<td>".$gP->img("asym_$vecbos_type_sfx$asym_type/hWBosonAsymAmpVsEta_$beam_sfx", false, 600)."\n";
      $row1 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span></div>\n";

      if ($beam_index == 0) {
         $row2 .= "<td>";
         $row4 .= "<td>";
         continue;
      }

      $row2 .= "<td>".$gP->img("asym_$vecbos_type_sfx$asym_type/mgrWBosonAsymVsPhi_EtaBins_$beam_sfx", false, 600)."\n";
      $row2 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span>, eta bins</div>\n";

   endforeach;

   print $row1;
   print $row2;
?>

</table>

</div>
<!-- }}} -->

<? endforeach; ?>

</div>
<!-- }}} -->


<h2 id="wbos_asym_vs_pt" class="count">W boson asymmetry vs. p_T</h2>
<!-- {{{ -->
<div id="div:wbos_asym_vs_pt" class="section">


<? foreach ($VECBOS_TYPE_SUFFIXES as $vecbos_type_idx => $vecbos_type_sfx): ?>

<h3 id="wbos_asym_vs_pt_<?=$vecbos_type_sfx?>" class="count"><?=$VECBOS_TYPE_HUMAN_DESCR[$vecbos_type_idx]?></h3>
<!-- {{{ -->
<div id="div:wbos_asym_vs_pt_<?=$vecbos_type_sfx?>" class="section">

<p>
<table class="simple00 cntr">

<?
   $row1 = "<tr>\n";
   $row2 = "<tr>\n";

   foreach ($RHIC_BEAM_SUFFIXES as $beam_index => $beam_sfx):

      $row1 .= "<td>".$gP->img("asym_$vecbos_type_sfx$asym_type/hWBosonAsymAmpVsPt_$beam_sfx", false, 600)."\n";
      $row1 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span></div>\n";

      if ($beam_index == 0) {
         $row2 .= "<td>";
         continue;
      }

      $row2 .= "<td>".$gP->img("asym_$vecbos_type_sfx$asym_type/mgrWBosonAsymVsPhi_PtBins_$beam_sfx", false, 600)."\n";
      $row2 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span>, p_T bins</div>\n";

   endforeach;

   print $row1;
   print $row2;
?>

</table>

</div>
<!-- }}} -->

<? endforeach; ?>

</div>
<!-- }}} -->


<h2 id="lepton_asym_vs_eta" class="count">Lepton asymmetry vs. pseudorapidity eta</h2>
<!-- {{{ -->
<div id="div:lepton_asym_vs_eta" class="section">


<? foreach ($VECBOS_TYPE_SUFFIXES as $vecbos_type_idx => $vecbos_type_sfx): ?>

<h3 id="wbos_asym_vs_eta_<?=$vecbos_type_sfx?>" class="count">Lepton, <?=$VECBOS_TYPE_HUMAN_DESCR[$vecbos_type_idx]?></h3>
<!-- {{{ -->
<div id="div:wbos_asym_vs_eta_<?=$vecbos_type_sfx?>" class="section">

<p>
<table class="simple00 cntr">

<?
   $row1 = "<tr>\n";
   $row2 = "<tr>\n";

   foreach ($RHIC_BEAM_SUFFIXES as $beam_index => $beam_sfx):

      $row1 .= "<td>".$gP->img("asym_$vecbos_type_sfx$asym_type/hLeptonAsymAmpVsEta_$beam_sfx", false, 600)."\n";
      $row1 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span></div>\n";

      if ($beam_index == 0) {
         $row2 .= "<td>";
         continue;
      }

      $row2 .= "<td>".$gP->img("asym_$vecbos_type_sfx$asym_type/mgrLeptonAsymVsPhi_EtaBins_$beam_sfx", false, 600)."\n";
      $row2 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span>, eta bins</div>\n";

   endforeach;

   print $row1;
   print $row2;
?>

</table>

</div>
<!-- }}} -->

<? endforeach; ?>

</div>
<!-- }}} -->


<h2 id="lepton_asym_vs_pt" class="count">Lepton asymmetry vs. p_T</h2>
<!-- {{{ -->
<div id="div:lepton_asym_vs_pt" class="section">


<? foreach ($VECBOS_TYPE_SUFFIXES as $vecbos_type_idx => $vecbos_type_sfx): ?>

<h3 id="wbos_asym_vs_pt_<?=$vecbos_type_sfx?>" class="count">Lepton, <?=$VECBOS_TYPE_HUMAN_DESCR[$vecbos_type_idx]?></h3>
<!-- {{{ -->
<div id="div:wbos_asym_vs_pt_<?=$vecbos_type_sfx?>" class="section">

<p>
<table class="simple00 cntr">

<?
   $row1 = "<tr>\n";
   $row2 = "<tr>\n";

   foreach ($RHIC_BEAM_SUFFIXES as $beam_index => $beam_sfx):

      $row1 .= "<td>".$gP->img("asym_$vecbos_type_sfx$asym_type/hLeptonAsymAmpVsPt_$beam_sfx", false, 600)."\n";
      $row1 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span></div>\n";

      if ($beam_index == 0) {
         $row2 .= "<td>";
         continue;
      }

      $row2 .= "<td>".$gP->img("asym_$vecbos_type_sfx$asym_type/mgrLeptonAsymVsPhi_PtBins_$beam_sfx", false, 600)."\n";
      $row2 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span>, p_T bins</div>\n";

   endforeach;

   print $row1;
   print $row2;
?>

</table>

</div>
<!-- }}} -->

<? endforeach; ?>

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
