<?php

include_once("config.php");


function htmlSpinStateYieldsTable(&$plotHelper, $phys_obj_type="wp", $hist="hWBosonPhiVsEta", $asym_type="")
{
   global $SPIN_STATES;

   $html = "<p>
   <table class='simple cntr'>
   ";

   // Create table header
   $html .= "<tr><th>\n";

   foreach ($SPIN_STATES as $spinYellow) {
      $html .= "<th>Yellow, $spinYellow\n";
   }

   foreach ($SPIN_STATES as $spinBlue)
   {

      $html .= "<tr><th>Blue, $spinBlue\n";

      foreach ($SPIN_STATES as $spinYellow)
      {
         if ($spinBlue == "0" && $spinYellow == "0") {
            $html .= "<td>&nbsp;\n";
            continue;
         }

         $beamSpinStates = $spinBlue.$spinYellow;

         $html .= "<td>".$plotHelper->img("asym/asym_$phys_obj_type$asym_type/{$hist}_$beamSpinStates", false, 400)."\n";
         $html .= "<p> ".$plotHelper->htmlLinkHistContents("asym/asym_$phys_obj_type$asym_type/{$hist}_$beamSpinStates.C")."\n";
      }
   }

   $html .= "</table>\n";

   return $html;
}


?>
