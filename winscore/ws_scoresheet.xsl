<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="html"/>
<xsl:template match="/SSA_Scoresheet">


<html>

<title><xsl:value-of select="Title"/> - <xsl:value-of select="Date/full"/></title>

<body>

<center>
  <H1><xsl:value-of select="Title"/>    -      <xsl:value-of select="Date/full"/></H1>
</center>


<hr></hr>


<xsl:for-each select="Class">

  <table cellpadding="13">
    <tr><td>
      <H2><xsl:value-of select="Name"/></H2>
    </td>
    <td>
      <H2><xsl:value-of select="Status"/></H2>
    </td>
    <td>
      <H2><xsl:value-of select="Task/Status"/></H2>
    </td>
    </tr>
  </table>


<table><tr><td  valign="top" >

  <table border="1" cellspacing="0" cellpadding="5">

    <tr bgcolor="#94ABF6">
        <th colspan="2" ><font face="Tahoma" size="2"><xsl:value-of select="Task/Type"/> Task</font></th>
    </tr>

   <xsl:choose>
     <xsl:when test="Task/Type = 'Turn Area'">
     <tr>
       <td colspan="1" ><font face="Tahoma" size="2">Min Distance</font></td>
       <td colspan="1" ><font face="Tahoma" size="2"><xsl:value-of select="Task/MinDistance"/></font></td>
      </tr>
     <tr>
       <td colspan="1" ><font face="Tahoma" size="2">Distance</font></td>
       <td colspan="1" ><font face="Tahoma" size="2"><xsl:value-of select="Task/Distance"/></font></td>
      </tr>

      <tr>
       <td colspan="1" ><font face="Tahoma" size="2">Max Distance</font></td>
       <td colspan="1" ><font face="Tahoma" size="2"><xsl:value-of select="Task/MaxDistance"/></font></td>
      </tr>
      <tr>
       <td colspan="1" ><font face="Tahoma" size="2">Min Time</font></td>
       <td colspan="1" ><font face="Tahoma" size="2"><xsl:value-of select="Task/MinimumTaskTime"/></font></td>
      </tr>
     <tr>
       <td colspan="1" ><font face="Tahoma" size="2">Task Open Time</font></td>
       <td colspan="1" ><font face="Tahoma" size="2"><xsl:value-of select="Task/TaskOpenTime"/></font></td>
      </tr>

     </xsl:when>
    </xsl:choose>

   <xsl:choose>
     <xsl:when test="Task/Type = 'Assigned'">
     <tr>
       <td colspan="1" ><font face="Tahoma" size="2">Distance</font></td>
       <td colspan="1" ><font face="Tahoma" size="2"><xsl:value-of select="Task/Distance"/></font></td>
      </tr>
     <tr>
       <td colspan="1" ><font face="Tahoma" size="2">Task Open Time</font></td>
       <td colspan="1" ><font face="Tahoma" size="2"><xsl:value-of select="Task/TaskOpenTime"/></font></td>
      </tr>
      </xsl:when>
    </xsl:choose>


   <xsl:choose>
     <xsl:when test="Task/Type = 'Mod. Assigned'">
     <tr>
       <td colspan="1" ><font face="Tahoma" size="2">Distance</font></td>
       <td colspan="1" ><font face="Tahoma" size="2"><xsl:value-of select="Task/Distance"/></font></td>
     </tr>
     <tr>
       <td colspan="1" ><font face="Tahoma" size="2">Min Time</font></td>
       <td colspan="1" ><font face="Tahoma" size="2"><xsl:value-of select="Task/MinimumTaskTime"/></font></td>
      </tr>
     <tr>
       <td colspan="1" ><font face="Tahoma" size="2">Task Open Time</font></td>
       <td colspan="1" ><font face="Tahoma" size="2"><xsl:value-of select="Task/TaskOpenTime"/></font></td>
      </tr>
      </xsl:when>
    </xsl:choose>

  </table>

 </td>
 <td valign="top">

  <table border="1" cellspacing="0" cellpadding="5">
    <tr bgcolor="#94ABF6">
       <th colspan="4" ><font face="Tahoma" size="2">Turnpoints</font></th>
    </tr>

    <tr bgcolor="#94ABF6">
       <th colspan="1" ><font face="Tahoma" size="2">#</font></th>
       <th colspan="1" ><font face="Tahoma" size="2">ID</font></th>
       <th colspan="1" ><font face="Tahoma" size="2">Name</font></th>
       <th colspan="1" ><font face="Tahoma" size="2">Radius</font></th>

    </tr>

    <tr>
    <td><font face="Tahoma" size="2">Start</font></td>
    <td><font face="Tahoma" size="2"><xsl:value-of select="Task/StartGate/ID"/></font></td>
    <td><font face="Tahoma" size="2"><xsl:value-of select="Task/StartGate/Name"/></font></td>
    <td><font face="Tahoma" size="2"><xsl:value-of select="Task/StartGate/Radius"/></font></td>
    </tr>

    <xsl:for-each select="Task/Turnpoints/Turnpoint">

	<tr>
	<td><font face="Tahoma" size="2">-</font></td>
	<td><font face="Tahoma" size="2"><xsl:value-of select="ID"/></font></td>
	<td><font face="Tahoma" size="2"><xsl:value-of select="Name"/></font></td>
	<td><font face="Tahoma" size="2"><xsl:value-of select="Radius"/></font></td>
	</tr>

    </xsl:for-each>

    <tr>
    <td><font face="Tahoma" size="2">Finish</font></td>
    <td><font face="Tahoma" size="2"><xsl:value-of select="Task/FinishGate/ID"/></font></td>
    <td><font face="Tahoma" size="2"><xsl:value-of select="Task/FinishGate/Name"/></font></td>
    <td><font face="Tahoma" size="2"><xsl:value-of select="Task/FinishGate/Radius"/></font></td>
    </tr>

  </table>

</td></tr></table>

  <table border="1" cellspacing="0" cellpadding="5">
    <tr><td>

      <tr bgcolor="#94ABF6">
        <th colspan="2" >
          <font face="Tahoma" size="2">
            Score Calculation Parameters
          </font>
        </th>
      </tr>

      <tr>
        <td colspan="1" >
          <font face="Tahoma" size="2">STD Min Task Dist.</font>
        </td>
        <td colspan="1" >
          <font face="Tahoma" size="2">
            <xsl:value-of select="SMTD"/>
          </font>
        </td>
      </tr>

      <tr>
        <td colspan="1" >
          <font face="Tahoma" size="2">STD Min Task Time</font>
        </td>
        <td colspan="1" >
          <font face="Tahoma" size="2">
            <xsl:value-of select="SMTT"/>
          </font>
        </td>
      </tr>

      <tr>
        <td colspan="1" >
          <font face="Tahoma" size="2">Scored Completion Ratio</font>
        </td>
        <td colspan="1" >
          <font face="Tahoma" size="2">
            <xsl:value-of select="SCR"/>
          </font>
        </td>
      </tr>
      <tr>
        <td colspan="1" >
          <font face="Tahoma" size="2">Short Task Factor</font>
        </td>
        <td colspan="1" >
          <font face="Tahoma" size="2">
            <xsl:value-of select="STF"/>
          </font>
        </td>
      </tr>
      <tr>
        <td colspan="1" >
          <font face="Tahoma" size="2">Max Speed Points</font>
        </td>
        <td colspan="1" >
          <font face="Tahoma" size="2">
            <xsl:value-of select="MSP"/>
          </font>
        </td>
      </tr>

      <tr>
        <td colspan="1" >
          <font face="Tahoma" size="2">Max Distance Points</font>
        </td>
        <td colspan="1" >
          <font face="Tahoma" size="2">
            <xsl:value-of select="MDP"/>
          </font>
        </td>
      </tr>

      <tr>
        <td colspan="1" >
          <font face="Tahoma" size="2">BESTDIST</font>
        </td>
        <td colspan="1" >
          <font face="Tahoma" size="2">
            <xsl:value-of select="BESTDIST"/>
          </font>
        </td>
      </tr>


      <tr>
        <td colspan="1" >
          <font face="Tahoma" size="2">Number Of Contestants</font>
        </td>
        <td colspan="1" >
          <font face="Tahoma" size="2">
            <xsl:value-of select="NumberOfContestants"/>
          </font>
        </td>
      </tr>
      
      <tr>
        <td colspan="1" >
          <font face="Tahoma" size="2">Number Of Finishers</font>
        </td>
        <td colspan="1" >
          <font face="Tahoma" size="2">
            <xsl:value-of select="NumberOfFinishers"/>
          </font>
        </td>
      </tr>
      
      <tr>
        <td colspan="1" >
          <font face="Tahoma" size="2">% Contestants > SMTD</font>
        </td>
        <td colspan="1" >
          <font face="Tahoma" size="2">
            <xsl:value-of select="ContestantsGTMinDist"/>
          </font>
        </td>
      </tr>

      <tr>
        <td colspan="1" >
          <font face="Tahoma" size="2">Best Speed</font>
        </td>
        <td colspan="1" >
          <font face="Tahoma" size="2">
            <xsl:value-of select="BestSpeed"/>
          </font>
        </td>
      </tr>

      <tr>
        <td colspan="1" >
          <font face="Tahoma" size="2">TOC Best Speed</font>
        </td>
        <td colspan="1" >
          <font face="Tahoma" size="2">
            <xsl:value-of select="TOCBestSpeed"/>
          </font>
        </td>
      </tr>

      <tr>
        <td colspan="1" >
          <font face="Tahoma" size="2">Best Distance</font>
        </td>
        <td colspan="1" >
          <font face="Tahoma" size="2">
            <xsl:value-of select="BestDist"/>
          </font>
        </td>
      </tr>

      <tr>
        <td colspan="1" >
          <font face="Tahoma" size="2">Num Finishers > 15min undertime</font>
        </td>
        <td colspan="1" >
          <font face="Tahoma" size="2">
            <xsl:value-of select="NumberOfFinishers15"/>
          </font>
        </td>
      </tr>  
      
   </td></tr>
 </table>
  
<p></p>


 <table border="1" cellspacing="0" cellpadding="0"> <tr><td>
 <table border="0" cellspacing="1" cellpadding="5">
  <tr bgcolor="#94ABF6">
    <th colspan="2" ><font face="Tahoma" size="2">Cumulative</font></th>
    <th colspan="3" ><font face="Tahoma" size="2">Pilot</font></th>

    <xsl:choose>
      <xsl:when test="Handicapped = 'Yes'">
        <th colspan="2" ><font face="Tahoma" size="2">Day</font></th>
        <th colspan="2" ><font face="Tahoma" size="2">Speed</font></th>
        <th colspan="2" ><font face="Tahoma" size="2">Distance</font></th>
        <th colspan="3" ><font face="Tahoma" size="2"></font></th>
      </xsl:when>
      <xsl:otherwise>
        <th colspan="7" ><font face="Tahoma" size="2">Day</font></th>
       </xsl:otherwise>
    </xsl:choose>

    <th colspan="3" ><font face="Tahoma" size="2">Penalty</font></th>
  </tr>

<tr bgcolor="#94ABF6">
<th><font face="Tahoma" size="2">Rank</font></th>
<th><font face="Tahoma" size="2">Points</font></th>
<th><font face="Tahoma" size="2">ID</font></th>
<th><font face="Tahoma" size="2">Name</font></th>
<th><font face="Tahoma" size="2">Glider</font></th>
<th><font face="Tahoma" size="2">Rank</font></th>
<th><font face="Tahoma" size="2">Points</font></th>

    <xsl:choose>
      <xsl:when test="Handicapped = 'Yes'">
        <th colspan="1" ><font face="Tahoma" size="2">Hndcp</font></th>
        <th colspan="1" ><font face="Tahoma" size="2">Actual</font></th>
        <th colspan="1" ><font face="Tahoma" size="2">Hndcp</font></th>
        <th colspan="1" ><font face="Tahoma" size="2">Actual</font></th>
      </xsl:when>
      <xsl:otherwise>
        <th colspan="1" ><font face="Tahoma" size="2">Speed</font></th>
        <th colspan="1" ><font face="Tahoma" size="2">Distance</font></th>
      </xsl:otherwise>
    </xsl:choose>

<th><font face="Tahoma" size="2">Start Time</font></th>
<th><font face="Tahoma" size="2">TOC</font></th>
<th><font face="Tahoma" size="2">TrnPts</font></th>
<th><font face="Tahoma" size="2">Code</font></th>
<th><font face="Tahoma" size="2">Points</font></th>

</tr>

    <xsl:for-each select="ScoreRecords/ScoreRecord">

      <tr>

        <xsl:choose>
        <xsl:when test="position() mod 2 = 1">
          <xsl:attribute name="bgcolor"></xsl:attribute> 
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="bgcolor">#cccccc</xsl:attribute> 
        </xsl:otherwise>
        </xsl:choose>
   
	<td><font face="Tahoma" size="2"><xsl:value-of select="CumRank"/></font></td>
	<td><font face="Tahoma" size="2"><xsl:value-of select="CPoints"/></font></td>
	<td><font face="Tahoma" size="2"><xsl:value-of select="CID"/></font></td>
	<td><font face="Tahoma" size="2"><xsl:value-of select="LastName"/>, <xsl:value-of select="FirstName"/></font></td>
	<td><font face="Tahoma" size="2"><xsl:value-of select="Glider"/></font></td>
	<td><font face="Tahoma" size="2"><xsl:value-of select="DayRank"/></font></td>
	<td><font face="Tahoma" size="2"><xsl:value-of select="Points"/></font></td>

        <xsl:choose>
           <xsl:when test="../../Handicapped = 'Yes'">
 	     <td><font face="Tahoma" size="2"><xsl:value-of select="HSpeed"/></font></td>
	     <td><font face="Tahoma" size="2"><xsl:value-of select="Speed"/></font></td>
 	     <td><font face="Tahoma" size="2"><xsl:value-of select="HDistance"/></font></td>
 	     <td><font face="Tahoma" size="2"><xsl:value-of select="Distance"/></font></td>
           </xsl:when>
           <xsl:otherwise>
  	     <td><font face="Tahoma" size="2"><xsl:value-of select="Speed"/></font></td>
 	     <td><font face="Tahoma" size="2"><xsl:value-of select="Distance"/></font></td>
           </xsl:otherwise>
         </xsl:choose>

  <td><font face="Tahoma" size="2"><xsl:value-of select="StartTime"/></font></td>
	<td><font face="Tahoma" size="2"><xsl:value-of select="TOC"/></font></td>
	<td><font face="Tahoma" size="2"><xsl:value-of select="AcheivedTurnpoints"/></font></td>
	<td><font face="Tahoma" size="2"><xsl:value-of select="Codes"/></font></td>
	<td><font face="Tahoma" size="2"><xsl:value-of select="Penalty"/></font></td>

     </tr>
    </xsl:for-each>

  </table>
  </td></tr>
  </table>

<p></p>

    
 <table border="1" cellspacing="0" cellpadding="0">
<tr><td>

 <table border="0" cellspacing="1" cellpadding="5">
  <tr bgcolor="#94ABF6">
    <th colspan="1" ><font face="Tahoma" size="2">Code</font></th>
    <th colspan="1" ><font face="Tahoma" size="2">Description</font></th>
  </tr>

   <xsl:for-each select="Codes/Code">
     <tr>

       <xsl:choose>
        <xsl:when test="position() mod 2 = 1">
          <xsl:attribute name="bgcolor"></xsl:attribute> 
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="bgcolor">#cccccc</xsl:attribute> 
        </xsl:otherwise>
      </xsl:choose>

	<td><font face="Tahoma" size="2"><xsl:value-of select="ID"/></font></td>
	<td><font face="Tahoma" size="2"><xsl:value-of select="Reason"/></font></td>
     </tr>
   </xsl:for-each>

  </table>

</td></tr>
</table> 
  
<p><hr></hr></p> 

</xsl:for-each>

<center>
Scoresheet created on <xsl:value-of select="CreationTime"/> by <A HREF="http://www.gfbyars.com/winscore">Winscore - <xsl:value-of select="ProgramVersion"/></A>
</center>
</body>
</html>



</xsl:template>
</xsl:stylesheet>
