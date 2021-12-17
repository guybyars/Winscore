<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="html"/>
<xsl:template match="/SSA_Summary">


<html>

<title><xsl:value-of select="Title"/> - Contest Summary </title>

  
  
<body>

<center>
  <H1><xsl:value-of select="Title"/> - Contest Summary
  <p></p><xsl:value-of select="Location"/></H1>
</center>



<xsl:for-each select="Classes/Class">

  <hr></hr>
  
  <H2><xsl:value-of select="Name"/></H2>


<table  border="1" cellspacing="1" cellpadding="5">

    <tr bgcolor="#94ABF6">
    <th colspan="6">Cumulative Score </th>

    <xsl:for-each select="Tasks/Task">
      <th colspan="2">
      <table>
        <tr>
        <th><font face="Tahoma" size="2">
          <xsl:value-of select="Date/Mon"/>  <xsl:value-of select="Date/day"/>,
          <xsl:value-of select="Date/yr"/>
        </font></th>
          </tr>
        <tr><th>
          <xsl:value-of select="Status"/>
          </th></tr>
         </table>
        </th>
    </xsl:for-each> 
    </tr>

  <tr bgcolor="#94ABF6">
    
    <th>Rank</th>
    <th>Points</th>
    <th>ID</th>
    <th>SSA#</th>
    <th>Name</th>
    <th>Glider</th>
    
    <xsl:for-each select="Tasks/Task">
      <th>Rank</th>
      <th>Points</th>
    </xsl:for-each>

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


      <xsl:choose>
        <xsl:when test="position() mod 10 = 1">
          <tr><td></td></tr>
        </xsl:when>
       </xsl:choose>



      <td>
        <font face="Tahoma" size="2">
          <xsl:value-of select="CumulativeRank"/>
        </font>
      </td>
      <td>
        <font face="Tahoma" size="2">
          <xsl:value-of select="CumulativePoints"/>
        </font>
      </td>
      <td>
        <font face="Tahoma" size="2">
          <xsl:value-of select="Contestant/CID"/>
        </font>
      </td>

      <td>
        <font face="Tahoma" size="2">
          <xsl:value-of select="Contestant/SSANumber"/>
        </font>
      </td>      
      
      <td>
        <font face="Tahoma" size="2">
          <xsl:value-of select="Contestant/LastName"/>, <xsl:value-of select="Contestant/FirstName"/>
        </font>
      </td>
      <td>
        <font face="Tahoma" size="2">
          <xsl:value-of select="Contestant/Glider"/>
        </font>
      </td>

      <xsl:for-each select="Days/Day">
        <td>
                 <font face="Tahoma" size="2">
                  <xsl:value-of select="Rank"/>
                </font>
              </td>
        <td>
          <font face="Tahoma" size="2">
            <xsl:value-of select="Points"/>
          </font>
        </td>        
      </xsl:for-each>

    </tr>
  </xsl:for-each>


</table>

  <p>
  </p>
  
</xsl:for-each>

  <p>
  </p>
  
<center>
Scoresheet created on <xsl:value-of select="CreationTime"/> by <A HREF="http://www.gfbyars.com/winscore">Winscore</A>
</center>
</body>
</html>



</xsl:template>
</xsl:stylesheet>
