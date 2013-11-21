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
include_once("utils.php");
include_once("PlotHelper.php");

$ana = isset($_GET['ana']) ? $_GET['ana'] : "run11_pp_transverse";
$vecbos_type = isset($_GET['vbt']) ? $_GET['vbt'] : "wp";
$asym_type = isset($_GET['at']) ? $_GET['at'] : "";
$asym_type = !empty($asym_type) ? "_$asym_type" : "";

$ana .= $vecbos_type == "z" ? "_-z" : "";

$dir  = "results/$ana/images";
$gP = new PlotHelper($dir);

$vecbos_hname = $vecbos_type == "z" ? "ZBoson" : "WBoson";

?>

<!-- Main text starts here-->

<h1>Plots</h1>

<div class="count" id="generated-toc"></div>

<hr>

<div id="main_text_body">
<!-- {{{ -->


<h1 id="asym_vs_rap" class="count">Boson asymmetry vs. rapidity</h1>
<!-- {{{ -->
<div id="div:asym_vs_rap" class="section">

<p>
<table class="simple00 cntr">

<?
   $row1 = "<tr>\n";
   $row2 = "<tr>\n";

   foreach ($RHIC_BEAM_SUFFIXES as $beam_index => $beam_sfx):

      $row1 .= "<td>".$gP->img("asym/asym_$vecbos_type$asym_type/h{$vecbos_hname}AsymAmpVsRap_$beam_sfx", false, 600)."\n";
      $row1 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span></div>\n";

      if ($beam_index == 0) {
         $row2 .= "<td>";
         continue;
      }

      $row2 .= "<td>".$gP->img("asym/asym_$vecbos_type$asym_type/mgr{$vecbos_hname}AsymVsPhi_RapBins_$beam_sfx", false, 600)."\n";
      $row2 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span>, rapidity bins</div>\n";

   endforeach;

   print $row1;
   print $row2;
?>

</table>


<h2 id="asym_vs_rap_yields" class="count">Yields</h2>
<!-- {{{ -->
<div id="div:asym_vs_rap_yields" class="section">

<?
print htmlSpinStateYieldsTable($gP, $vecbos_type, "h{$vecbos_hname}PhiVsRap", $asym_type);
?>

</div>
<!-- }}} -->


</div>
<!-- }}} -->


<h1 id="asym_vs_eta" class="count">Boson asymmetry vs. pseudorapidity eta</h1>
<!-- {{{ -->
<div id="div:asym_vs_eta" class="section">

<p>
<table class="simple00 cntr">

<?
   $row1 = "<tr>\n";
   $row2 = "<tr>\n";

   foreach ($RHIC_BEAM_SUFFIXES as $beam_index => $beam_sfx):

      $row1 .= "<td>".$gP->img("asym/asym_$vecbos_type$asym_type/h{$vecbos_hname}AsymAmpVsEta_$beam_sfx", false, 600)."\n";
      $row1 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span></div>\n";

      if ($beam_index == 0) {
         $row2 .= "<td>";
         continue;
      }

      $row2 .= "<td>".$gP->img("asym/asym_$vecbos_type$asym_type/mgr{$vecbos_hname}AsymVsPhi_EtaBins_$beam_sfx", false, 600)."\n";
      $row2 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span>, eta bins</div>\n";

   endforeach;

   print $row1;
   print $row2;
?>

</table>


<h2 id="asym_vs_eta_yields" class="count">Yields</h2>
<!-- {{{ -->
<div id="div:asym_vs_eta_yields" class="section">

<?
print htmlSpinStateYieldsTable($gP, $vecbos_type, "h{$vecbos_hname}PhiVsEta", $asym_type);
?>

</div>
<!-- }}} -->


</div>
<!-- }}} -->


<h1 id="asym_vs_pt" class="count">Boson asymmetry vs. P_T</h1>
<!-- {{{ -->
<div id="div:asym_vs_pt" class="section">

<p>
<table class="simple00 cntr">

<?
   $row1 = "<tr>\n";
   $row2 = "<tr>\n";

   foreach ($RHIC_BEAM_SUFFIXES as $beam_index => $beam_sfx):

      $row1 .= "<td>".$gP->img("asym/asym_$vecbos_type$asym_type/h{$vecbos_hname}AsymAmpVsPt_$beam_sfx", false, 600)."\n";
      $row1 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span></div>\n";

      if ($beam_index == 0) {
         $row2 .= "<td>";
         continue;
      }

      $row2 .= "<td>".$gP->img("asym/asym_$vecbos_type$asym_type/mgr{$vecbos_hname}AsymVsPhi_PtBins_$beam_sfx", false, 600)."\n";
      $row2 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span>, p_T bins</div>\n";

   endforeach;

   print $row1;
   print $row2;
?>

</table>


<h2 id="asym_vs_pt_yields" class="count">Yields</h2>
<!-- {{{ -->
<div id="div:asym_vs_pt_yields" class="section">

<?
print htmlSpinStateYieldsTable($gP, $vecbos_type, "h{$vecbos_hname}PhiVsPt", $asym_type);
?>

</div>
<!-- }}} -->


</div>
<!-- }}} -->

<?
// Skip the lepton part for the Z boson
if ($vecbos_type == "wp" || $vecbos_type == "wm"):

$vecbos_index = array_search($vecbos_type, $VECBOS_TYPE_SUFFIXES);
$lepton_sfx = $LEPTON_TYPE_SUFFIXES[$vecbos_index];
?>

<h1 id="lepton_asym_vs_eta" class="count">Lepton asymmetry vs. pseudorapidity eta</h1>
<!-- {{{ -->
<div id="div:lepton_asym_vs_eta" class="section">

<p>
<table class="simple00 cntr">

<?
   $row1 = "<tr>\n";
   $row2 = "<tr>\n";

   foreach ($RHIC_BEAM_SUFFIXES as $beam_index => $beam_sfx):

      $row1 .= "<td>".$gP->img("asym/asym_$lepton_sfx$asym_type/hLeptonAsymAmpVsEta_$beam_sfx", false, 600)."\n";
      $row1 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span></div>\n";

      if ($beam_index == 0) {
         $row2 .= "<td>";
         continue;
      }

      $row2 .= "<td>".$gP->img("asym/asym_$lepton_sfx$asym_type/mgrLeptonAsymVsPhi_EtaBins_$beam_sfx", false, 600)."\n";
      $row2 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span>, eta bins</div>\n";

   endforeach;

   print $row1;
   print $row2;
?>

</table>


<h2 id="lepton_asym_vs_eta_yields" class="count">Yields</h2>
<!-- {{{ -->
<div id="div:lepton_asym_vs_eta_yields" class="section">

<?
print htmlSpinStateYieldsTable($gP, $lepton_sfx, "hLeptonPhiVsEta", $asym_type);
?>

</div>
<!-- }}} -->


</div>
<!-- }}} -->


<h1 id="lepton_asym_vs_pt" class="count">Lepton asymmetry vs. P_T</h1>
<!-- {{{ -->
<div id="div:lepton_asym_vs_pt" class="section">

<p>
<table class="simple00 cntr">

<?
   $row1 = "<tr>\n";
   $row2 = "<tr>\n";

   foreach ($RHIC_BEAM_SUFFIXES as $beam_index => $beam_sfx):

      $row1 .= "<td>".$gP->img("asym/asym_$lepton_sfx$asym_type/hLeptonAsymAmpVsPt_$beam_sfx", false, 600)."\n";
      $row1 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span></div>\n";

      if ($beam_index == 0) {
         $row2 .= "<td>";
         continue;
      }

      $row2 .= "<td>".$gP->img("asym/asym_$lepton_sfx$asym_type/mgrLeptonAsymVsPhi_PtBins_$beam_sfx", false, 600)."\n";
      $row2 .= "<div class='thumbcaption_cm'><span class=bluPol>{$RHIC_BEAM_HUMAN_DESCR[$beam_index]}</span>, P_T bins</div>\n";

   endforeach;

   print $row1;
   print $row2;
?>

</table>


<h2 id="lepton_asym_vs_pt_yields" class="count">Yields</h2>
<!-- {{{ -->
<div id="div:lepton_asym_vs_pt_yields" class="section">

<?
print htmlSpinStateYieldsTable($gP, $lepton_sfx, "hLeptonPhiVsPt", $asym_type);
?>

</div>
<!-- }}} -->


</div>
<!-- }}} -->

<?
endif;
?>


<!-- Main text ends here-->
</div>
<!-- }}} -->


<?php
include("bottom.html");
?>

</body>
</html>
