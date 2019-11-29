<?php
require_once('Date/Holidays.php');
$today = new Date;
$holidays = Date_Holidays::factory('USA');
$hol = $holidays->isHoliday($today);
$dow = $today->getDayofWeek();
$isWorkday = ($dow !=0 and $dow != 6 and !hol);
$os = $isWorkday ? $today->getDate() . " is a workday\n" : $today->getDate() . " is not a workday\n";
#echo $os;
$os2 = $isWorkday ? "true" : "false";
echo $os2;
?>
