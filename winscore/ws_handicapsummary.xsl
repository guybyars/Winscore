<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="html"/>
<xsl:template match="/Winscore">


<html>

<title>Handicapping Data Summary</title>

<body>

<table>
  <tr><td align="center">
  <font face="Tahoma" size="5">Handicapping Data Summary</font>
  <p></p>  
  <font face="Tahoma" size="4">
    <xsl:value-of select="startdate/yr"/> - <xsl:value-of select="Title"/>, <xsl:value-of select="Location"/>
</font>  
  <p></p>
</td></tr>
  <tr><td>
 <table border="1" cellspacing="0" cellpadding="5">

   <tr bgcolor="#94ABF6">
     <th colspan="4" ><font face="Tahoma" size="2"></font></th>
     <th colspan="6" ><font face="Tahoma" size="2">SSA Reference Configuration Data</font></th>
     <th colspan="3" ><font face="Tahoma" size="2">Non Factory</font></th>
     <th colspan="2" ><font face="Tahoma" size="2">Contest</font></th>
     <th colspan="2" ><font face="Tahoma" size="2"></font></th>
   </tr> 
   
   <tr bgcolor="#94ABF6">
      <th colspan="1" ><font face="Tahoma" size="2">CID</font></th>
      <th colspan="1" ><font face="Tahoma" size="2">First Name</font></th>
      <th colspan="1" ><font face="Tahoma" size="2">Last Name</font></th>
      <th colspan="1" ><font face="Tahoma" size="2">Class</font></th>
      <th colspan="1" ><font face="Tahoma" size="2">Glider</font></th>
      <th colspan="1" ><font face="Tahoma" size="2">Motor</font></th>
      <th colspan="1" ><font face="Tahoma" size="2">Weight</font></th>
      <th colspan="1" ><font face="Tahoma" size="2">Span</font></th>
      <th colspan="1" ><font face="Tahoma" size="2">Hndcp</font></th>
      <th colspan="1" ><font face="Tahoma" size="2">Wlets</font></th>
      <th colspan="1" ><font face="Tahoma" size="2">Wlets</font></th>
      <th colspan="1" ><font face="Tahoma" size="2">Turbs</font></th>
      <th colspan="1" ><font face="Tahoma" size="2">Fairing</font></th>
      <th colspan="1" ><font face="Tahoma" size="2">Weight</font></th>
      <th colspan="1" ><font face="Tahoma" size="2">Span*</font></th>
      <th colspan="1" ><font face="Tahoma" size="2">Hndcp</font></th>
      <th colspan="1" ><font face="Tahoma" size="2">%</font></th>     
    </tr>

    <xsl:for-each select="Contestants/Contestant">
      <tr>
        <xsl:choose>
        <xsl:when test="position() mod 2 = 1">
          <xsl:attribute name="bgcolor"></xsl:attribute> 
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="bgcolor">#cccccc</xsl:attribute> 
        </xsl:otherwise>
        </xsl:choose>
        <td align="center"><font face="Tahoma" size="2"><xsl:value-of select="CID"/></font></td>
	      <td><font face="Tahoma" size="2"><xsl:value-of select="FirstName"/></font></td>
        <td>
          <font face="Tahoma" size="2">
            <xsl:value-of select="LastName"/>
          </font>
        </td>
        <td>
          <font face="Tahoma" size="2">
            <xsl:value-of select="Class"/>
          </font>
        </td>
        <td><font face="Tahoma" size="2"><xsl:value-of select="Glider"/></font></td>
        <td align="center">
          <xsl:if test="contains(gliderinfo/Notes,'M')">Yes</xsl:if>
          <xsl:if test="not(contains(gliderinfo/Notes,'M'))"></xsl:if>
        </td>
        <td align="center"><font face="Tahoma" size="2"><xsl:value-of select="gliderinfo/Weight"/></font></td>
        <td align="center"><font face="Tahoma" size="2"><xsl:value-of select="gliderinfo/Span"/></font></td>
        <td align="center"><font face="Tahoma" size="2"><xsl:value-of select="gliderinfo/Handicap"/></font></td>
        <td align="center">
          <xsl:if test="contains(gliderinfo/Notes,'W')">Yes</xsl:if>
          <xsl:if test="not(contains(gliderinfo/Notes,'W'))"></xsl:if>       
        </td>
        <td align="center">
          <xsl:choose>  <!-- Non Factory Wlets Check bit 6--> 
          <xsl:when test="floor(Options div 32) mod 2 = 1">Yes</xsl:when>
            <xsl:otherwise></xsl:otherwise>
          </xsl:choose>
        </td>
        <td align="center">
          <xsl:choose> <!-- Non Factory Turbs Check bit 7-->
            <xsl:when test="floor(Options div 64) mod 2 = 1">Yes</xsl:when>
            <xsl:otherwise></xsl:otherwise>
          </xsl:choose>
        </td>
        <td align="center">
          <xsl:choose>
          <!-- Non Factory Fairing Check bit 8-->
            <xsl:when test="floor(Options div 128) mod 2 = 1">Yes</xsl:when>
            <xsl:otherwise></xsl:otherwise>
          </xsl:choose>
        </td>        
        <td>

            <!--Weight if different-->
              <font face="Tahoma" size="2">
                <xsl:value-of select="Weight"/>
              </font>
              <font face="Tahoma" size="1">			  
      				  <xsl:if test="WeightDelta!=0.0">
			      		  (<xsl:value-of select="WeightDelta"/>)
				        </xsl:if>
              </font>

        </td>
        
        <td>
          <xsl:choose>    <!--Span if different-->
            <xsl:when test="not(Span=gliderinfo/Span)">
              <font face="Tahoma" size="2"><xsl:value-of select="Span"/></font>
            </xsl:when>
            <xsl:otherwise></xsl:otherwise>
          </xsl:choose>
        </td>

        <td><font face="Tahoma" size="2"><xsl:value-of select="Handicap"/></font></td>
        <td><font face="Tahoma" size="2"><xsl:value-of select="HandicapPercent"/></font></td>
        
      </tr>
    </xsl:for-each>
   
   <tr><td colspan="17">* Contest Span and Weight only shown if different from Reference</td></tr>
   
  </table>
  </td></tr>
  <tr><td align="center">
  Handicap Summary created on <xsl:value-of select="CreationTime"/> by <A HREF="http://www.gfbyars.com/winscore">Winscore</A>  
  </td></tr>
  
</table>
  
</body>
</html>


</xsl:template>
</xsl:stylesheet>
