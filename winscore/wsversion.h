#if !defined(WSVERSION_H_INCLUDED_)
#define WSVERSION_H_INCLUDED_

//#define	WINSCOREBETA         1
#define CURRENTYEAR			 2025
#define WINSCOREVERSION		_T("2025 Rev 1a")
#define SHORTVERSION			 _T("Rev 1a")
#define WINSCOREWEB			_T("www.gfbyars.com/winscore")


/*	Version Notes:
2024

Beta 0
	* No changes from 2023 Rev 5.
Rev 1
   * Removed handicap normalization for FAI tasks.

Rev 2
   * Disabled better start warning
   * Check ACTIVE warnings when double-clicking a flight
   * Added more info in invalid turn area turnpoint specification
   * Fixed file open failure when .wscx file is on google drive folder

Rev 3
   * Copied IGC file to local temp directory for validateion.  Solves problem with virtural/Google drives that some manufactures dll don't handle.
   * Fixed task open time being used from imported tasks
   * ignored character upper/lower case differences in Manufacturers in Handicap list.
   * Fixed program error when no crossing of start line for FAI task.

Rev 4
   * Added Scoring Parameters for FAI tasks in XML scoresheet file.

Rev 5
   * Corrected SMTD/2 distance check to 5.0 miles "Rule 11.2.3.5 Scored Distance"

Rev 6
   * Corrected "Humorous" FAI 7.4.5b altitude penalty when no start was acheived
   * Corrected start line detection when course line is nearly parallel to start line.
   * Output start radius to scoresheets when line time is specified.
   * Updated Handicap Summary Report Formatting
   * Added exporting of tasks in CUP format

Rev 7
   * Increased tolerance for fix interval >1 second warning
   * Corrected bugs in CUP task export

Rev 8
   * Updated CUP task export to support WeGlide

Rev 9
   * Corrected score roundoff issue with FAI tasks
   * Corrected SP0 in parameter output to SSA

Rev 10
   * Enhancements to CUP file export
   * Corrected Spm in parameter output to SSA when no flights are present for the day.

Rev 11 & 12
   * Corrected scoring errors in FAI tasks.

2023
  * Corrected issue where incorrect case in handicap list was causing Discus 2b to be selected.

Rev 5
	* Added PEV Starts
	* Enabled finish line base altitude specification

Rev 4
	* Added informational message for motor runs before starts

Rev 3 
	* Fixed handicapping distance computation for FAI tasks.

Rev 2
   * Changed motor detection tolerance to reduce false alarms
   * Added 1s fix interval check for FAI tasks.
   * Added altitude and speed start checks for FAI tasks.

Rev 1
   * No change from 2022 version

2022

Rev 7
   * Corrected issue with only 1 practice day
   * Corrected processing of start time edit when the pilot never entered the start cylinder.

Rev 6
   * Corrected processing of long files with 6 character SN field

Rev 5
   * Corrected missing Airfield Bonus for motorglider engine run
   * When duplicate logs encountered, give preference to log with good ENL trace.

Rev 4
   * Corrected ENL & MOP display when no task was defined.
   * Fixed crash when no valid start occured.
   * Added warning ifArtificial horizon on/off (BFION/BFIOFF)

Rev 3 
   * Corrected ENL display when no task was defined.

Rev 2 
   * Updated tree window visibility
   * Update for Contestant Factor
   * Updated for new contestant criterion 11.1.1


Rev 1 
   * Code cleanup, move to GitHub
   * Rmoved Cambridge Logger code
   * Added user defined number of practice days
   * put practice days under separate tree item "P"
   * Removed obsolete control bar code.

Beta
   * No change from 2021 Rev 15

2021
Rev 15
	* Restored turnpoint warning
	* Corrected motor run bonus criteria and warning message
	* Fixed issue with user input flight log directories

Rev 13 & 14
    * Corrected FAI start radius subtraction for incomplete flights.

Rev 12
    * Corrected FAI start radius subtraction.

Rev 11
    * Corrected FAI handicap application

Rev 10
    * Added FAI start penalty check

Rev 9
    * Corrected date rollover which occurs on the 31st of the month.

Rev 8
    * Corrected FAI distance calculations for Area Tasks

Rev 7
    * Updated furthest progress computation on FAIArea tasks to comply with FAI rules.

Rev 6
    * Corrected legacy bug in SSA Assigned task score calculations
	* Corrected airfield bonus award

Rev 5
    * Corrected landout distance to conform to FAI Bullshit.
	* Added airfield bonus points to final score, not the provisional


Rev 4
    * Corrected STOC to alway use MINTIME for FAI & *HANDICAPPED* classes
	* Permitted .5 radius turn areas.

Rev 3
    * Fixed spuirous warning about task lengths for FAI tasks.
	* Toned down the FAI Experimental task nag message for task creation.
	* No automatic airport bonus for FAI with distance < 100km
	* Added % complete (N/Dm) for FAI task information output
	* Corrected FAI max scores from being negative when 0 constants started.

Rev 2
	Updates for FAI scoring
    * Corrected STOC to alway use MINTIME for FAI
	* Implemented alternate FAI scoring

Rev 1
	Updated to access members.ssa.org

2020

Rev 1
   Corrected score upload to the new members.ssa.org.

2020

   Rev 2
   Corrected leg distance display to account for out the back of the cylinder check.  Scored distances were correct.

   Rev 1
   Restored out the back of the start cylinder check.

2019
   Rev 11
   Permitted editing to a start with a penalty.

2019
   Rev 10
   Removed any finish penalty warnings when motor run landout is made.
   Assigned Airfield status to turnpoints read in with the "L" attribute.
   Added Checked and OK to log status.
   Corrected issue where erased 2ndary SUA airspace file name was retained.

2019
   Rev 9
   Corrected issues with FAI tasks.
   Added a "FAI Settings..." tab to the class properties which include D1 & Dm

2019
   Rev 8
   Corrected issues with FAI tasks & start lines.

/*	Version Notes:
2019
   Rev 7
   Corrected issues with FAI tasks & start lines.
   Made the airfield bonus user modifiable.

2019
   Rev 6
   Fixed flight log->Info... output to display correct first leg length.
   Made the FAI Sphere the default Datum option.
   Added experimental FAI tasks.
   Corrected issue where bogus task would sometimes appear by default.
   Fixed E/W Longitude bug.
   Fixed non-miles units processing in fight display.

2019
   Rev 5
	Added File Save reminder
	Removed start penalty tolerance.
	Changed tolerance to detect turnoint acheivement by looking at next turnoint entry instead of its outer ring
	Corrected issue with "Edit->Change turnpoint fix" not displaying edit properly.
	Corrected issue with CAI 302 security check failures.
	Corrected spelling error: Mark Security Pased
	Fixed motor run detection failure when contestant turns motor on and keeps it on and enters finish cylinder.
	Fixed bogus file name creation such as "2000-00-00-FLA-XXX-01" due to duplicate HDTE records

   Rev 4
	Fixed Gate initialization error

2019
   Rev 3
	Changed handicap weight adjustment to reflected updated SSA rules.

2019
   Rev 2
   Changed Datum from FAI Sphere to WGS84, and added option to switch between the two
   Added a orient perpendicular to course switch to start line option.
   Added experimental FAI Racing task
   Added flag to permit out the top starts.  This is always on by default.
   Corrected duplicate HDTE records
   Changed TP radii back to 1 and 2

2019
   Rev 1
   Changed TP radii to .5 and 1.5
   Fixed SUA airspace releative name with no wscx file created.
   Updated Finish Penalty to 2019 rules.
   Updated MotorGlider airfield parameters, 1 mile and 800 ft.
   Updated Handicap Weight formula, removed turbulation adjustment.

2018
   Rev 17
   Used ssl and port 443 when uploading to www.ssa.org.

   Rev 16
   Access www.ssa.org via "https" instead of "http"
   Corrected SUA display for certain ANTI-CLOCKWISE segements.
   Corrected non-US units conversion on flight display.
   Output improved warning when a turnpoint file with an invalid turnpoint ID is read in.

2018
   Rev 15
   Corrected typo in SSA contestant download warning.
   Added check for reasonable handicap values in Contestant entry dialog.
   Added handicap value lookup and update when the glider selection combo is changed on contestant dialog.

2018
   Rev 14
   Fixed 25 point error in start penalty recomendation.
   Included actual distance in SMTD/2 error message.
   If scoresheet submission in autoscore failed, then retry to send on the next cycle.

2018
   Rev 13
   Let autoscore continue in the event of an error, but 5 erors and you're out.
   Added additional filter to motor run detection
   Fixed possible crash when analyze all is done, then the 2nd of a multiple flight log was selected.

2018
   Rev 12
   Add provision for 2ndary SUA airspace file.
   Updated error message when contestants with duplicate CIDs were downloaded from ssa.org

2018
   Rev 11
   Made Flight Log .zip export file names consistent and unique.
   Fixed version ID when converting IGC files to long file names during import.
   Fixed "guests at bottom of scoresheet" option

2018
   Rev 10
   Corrected corrected altitude computation when crossing finish gate line.
   Fixed intermittent crash when double flight logs appear in flight list.
   Lowered tolerance to flag motor runs more reliably, but increase the false positives.

2018
   Rev 9
   Corrected corrected altitude computation to be compliant with rule 11.2.1.5.1  .

2018
   Rev 8
   <technobable> Added CoInitialize() call to autoacore thread </technobable>.

2018
   Rev 6 & 7
   Added additional diagnostics if scoresheet failes in autoexport.

2018
   Rev 5 
   Corrected absence of certain scoring parameters on scoresheet for practice day.
   Added additional diagnostics if scoresheet failes in autoexport.

2018
   Rev 4 (Critical Program update!)
   Changed text of Finish penalty warning in warning resolution box
   Warned if contestants duplicate CIDs were imported from ssa.org
   Changed the airfield bonus award radius to 1.00 statute miles. 
   Deactivated obsolete checkbox option on Start Gate dialog

2018
   Rev 3
   Fixed bogus finish penalty recomendation.
   Added program version output to .wscx file.

2018
   Rev 2
   Fixed Airfield Bonus when scored distance is 0.
   Fixed Airfield Bonus when motor run

2017
   Beta 7
   Updated rule 11.6.6 Scored Time on Course.
   Corrected application of contest penalties on Practice Days.

   Rev 1
		Removed timestamp verification for flight logs
		Corrected saved password retrevial
		Corrected FAI Handicapped class import
		Corrected Start time edit after 1st turnpoint was acheived.

   Rev 2
		Fixed bug in contestant import from xml
		Added Club Class
		Accepted as OK when SSA server returns "Bad File Type"
		Added the current wscx file to the flight log .zip file

   Rev 3
		Fixed TAT Radii going out for Assigned Tasks
		Changed nomenclature from Assigned Speed Task to Assigned Task.
		Fixed problem where check start diagnositcs were not displayed when checking a single flight.
		Fixed Sharing violation during autoscore.

   Rev 4
		Added No Log Found text override function.
		Made finishing below finish cylinder an incomplete task.

   Rev 5
		Recognized MOP entry in IGC files for method of propulsion in addition to ENL.
		Fixed contest setup regional/national radio flag setup for National contests

   Rev 6
		Improved recognition of start time which yields the best speed.
		Fixed xml name formulation when exporting flight logs to ssa.org

   Rev 7
		Improved recognition of start time which yields the best speed when a start penalty was applied or latest start was before task open time.
		Fixed issues with Alternate Start time selection

   Rev 8
		Continuous improvement in recognition of start time which yields the best speed.

   Rev 9
		Made flight log paths and SUA file path relative to the .wscx file.

   Rev 10
		Corrected default SMTD and SMTT for several classes.
		Corrected Club Class Handicap update during inport.
		Imposed handicap limit check for Club class during import.
		Fixed Auto Score time interval persistance.
		Changed format of text when copying a task to the clipboard

   Rev 11
		Made the auto score status log persist between sessions
		Made the no log found status appear sorted, but a refresh is required
		Corrected Autoscore to identify non contestant logs via email.

   Rev 12
		Corrected MAXDP for assigned tasks when there are no finishers

   Rev 13
		Corrected the withdrew status not being saved properly
		When no fix is in start cylinder, then only those fixes after task open are considered for start fix.
		Changed task setup variables to be stored in local registery, not system registery.

   Rev 14
		Trapped invalid B records in log file

   Rev 15
		Added warning message if an edit or an analysis is attempted on a contest day which has been made official.
		Added Autosave checkbox in AutoScore.
		Stored contest passwords in the computer registery, not the data file.
		Indicated with an * the specific values which were edited in the flight log list.
		Corrected the best start determination for distance only non finishers.
		Corrected the start determination and penalty when the latest starte is before the task opens.

 Rev 16
		Improved MAT turnpoint edits and detection
		Removed * from the specific flight values which were exported to scoresheets.
		Fixed edited flight not being displayed correctly after a save.
	
 Rev 17
		Fixed a critial bug introduced in Rev 16.  Do not use Rev 16, it is Abby-Normal.

 Rev 18
		Improved start detection.
		Made Edit... flights more reliable.

 Rev 19
		Fixed crash when no points in start cylinder after task open time

 Rev 20
		Improved motor run detection
		Implemented altitude detection for finish gates

 Rev 21
		Implemented statistical analysis in motor run detection
		Added airfield bonus for motor retrieve.
		Added rolling finish when finish gate is in use.
		Added Copy capability from View Analysis dialog.


 Rev 22
		Fixed bug in selecting best scoring start
		Fixed bug in start time edit


Rev 25
		Added Auto management of flight logs by date.

2016
  Rev 1 - Fixed double dipping of the contest penalties.
          Streamlined the contestant import from the SSA.

  Rev 2 - Made Standard Class handicapped by default
		  Fixed MAT analysis problem where tasks cut short were scored as incomplete
		  Added some convenience pre defined penalty codes
		  Fixed incorrect display of BESTDIST in the scoring info output
		  Fixed inclusion of superious turnpoints in the turnpoint edit box for MATS
		  Fixed installer to put several manufacturer supplied security dlls in the correct system directory
		  Changed security check to first check for later "igc-*2.dll" versions.

  Rev 3 - Handicap List is now read real time from the SSA website, not a local file
          Added logic to automtically select the latest penalty free start (not guarenteed to produce the best score)
		  Added logic to automatically land out motorgliders who do an engine retreive.

  Rev 4-	Correct SMTD for standard class in regional contests from 50 to 40 miles.
			Added additional manufacturer dlls to the installer
			Fixed pass/fail toggle in flight log display

2015
  Rev 1 - Increased max turnpoint ID from 500 to 999
  Rev 0 - Upgrade for 2014 Rules, changed safety finish from 60 to 90 mph.

2014
  Rev 0 - Upgrade for 2014 Rules
  Rev 1 - Increased line length buffer for reading IGC files.
  Rev 2 - Increased XML buffer to prevent crash when garbage data is converted to int xml fields
		  Added newline char when copying task data to paste buffer
		  Added Min Time output to Turn Area Task task sheets.
  Rev 3 - Fixed incorrect scores applied to guest finisher when only guests finished.

2013
  Rev 0 - Upgrade for 2013 Rules

2012
  Rev 0 - Upgrade for 2012 Rules
  Rev 1 - Fixed SUI display bug
  Rev 2 - Fixed Start line display bug
  Rev 3 - Fixed BESTDIST when non finishers went further than finishers.
  Rev 4 - Fixed BESTDIST for handicapped classes
  Rev 5 - Fixed Flight Log Edit zeroing out distance when selecting different starts.
  Rev 6 - Fixed "Invalid Parameter" popup when a fix with Bogus time value was read.
  Rev 7 - Fixed FAI class using dist>SMTD for finish check instead of hdcapped dist>SMTD

2011
  Rev 0 - Updgraded to Visual Studio 2011 & New UI
  Rev 1 - Fixed Cumulative scoresheet not uploading to SSA server.
  Rev 2 -	Re added import/export contestant in xml menus
			Permitted CID change in new contestant edit/add dialog
  Rev 3 -   Removed spaces from start time text in xml and elsewhere.
  Rev 5 -	Updated security checking to take place in a separate thread
			Updated CAI security checking
  Rev 6 -	Corrected Airspace display for clockwise and anti-clockwise sua segments
  Rev 7 -	Corrected KM units display on task setup graphics

2010
  Rev 0 - Updgraded to Visual Studio 2003
  Rev 1 - Fixed SUA text string copy bug that caused a "This parameter is not correct" exception.
        - Added more robust exception processing.
  Rev 2 - Fixed Start Penalty not adding 25 points.
        - Fixed CAM check.  Added m_strHardware to Archive/XML
  Rev 3 - Fixed Start penalty recommendation
        - Added MSL to start/finish cylinder altitudes.
        - Fixed scoring formula for sports nationals

2009
  Rev 0 - 6-Mar-09	-	Update for '09.
							- Long IGC file names
							- New Finish Rules
							- Added more columns to view analysis giving more altitude information

  Rev 1 - 8-Mar-09	-	Added XML Export Capability

  Rev 2 - 09-Mar-09	-	Fixed min time entry bug
						Added more error checking to XML Export
						Fixed Start penalty calculation bug

  Rev 3 - 10-Mar-09	-	Fixed edit bug not computing start point properly.
 

  Rev 4 - 5-April-09-	Fixed Finish penalty code not to recommend neg penalties
						Added FP-Finish Penalty code to Penalty dl (Removed Landing Card Penalty).

  Rev 5 - 8-May-09 -    Fixed bugs in handicap lookup.
						Fixed bug in displaying finish penalty warning

  Rev 6 - 8-May-09 -    Fixed (disabled) auto export path check, It wasn't working on network drives.

  Rev 6a- 28-May-09 -   Allowed Alternate Starts when task incomplete.
  
  Rev 7 - ???		-	Fixed HH.MM.SS format on control point printing.

  Rev 8 - 21-Jul-09	-	Fixed long igc name import problem.  Added support for generic manufacturer security dlls


2008
  Rev 1 -  27/Jun/08  - Fixed incorrect points when noone finished
						Fixed 5000mi+ distance when contestant never left start cyl.
						
  Rev 0 -  8/Mar/08   - Added the Start Anywhere start cylinder
						Added the Finish Penalty
						Changed Nat scoring formula to be the same as regional.
						Updated the Handicap calculator for new handicap rules.
						Fixed various bugs.
						

2007

  Rev 4 -  14/Jul/07   -Added Safety Finish.  
						Added better way to find handicaplist.xml.

  Rev 1 -  11/Mar/07   -Fixed Foreign pilot status for motorgliders
						Added XML scoresheet export.

  Rev 0 -  1/Mar/07    -Update for '07


2006

  Rev 0 -  4/Mar/06     -Update for '06

  Rev 10 -  20/Sept/05	-Fixed A drive hang on Win98

  Rev 9 -  07/Jul/05	-Added overlaping TAT task Check
						-Added better landing detection
						-Fixed scored landing location display in flight/edit.

  Rev 8 -  ??/???/05	-Fixed Rev 7 bug.

  Rev 7 -  ??/???/05	-Added XML support
						-Fixed Altitude warning
						-Added flashcard import.

  Rev 6 -  23/May/05	-Fixed Scoring Formula.

  Rev 5 -  21/May/05	-Fixed Scoring Formula.

  Rev 3 -  7/May/05	    -Added Log condenser
						-Fixed improved roll time altitude selection.

  Rev 2 -  6/May/05	    -Added Hdcap Weight calculator fix.
						-Added airspace check for above airspace
						-Added improved roll time altitude selection.

  Rev 0 -  26/Feb/05   -Added Alternate start selector

  Rev 9 -  16/Sept/04  -Refined FTP U/I

  Rev 8 -  14/Sept/04  -Enhanced FTP processing

  Rev 7 -  10/Sept/04  -Added Task info copy to clipboard.

  Rev 6 -  28/Aug/04   -Enhanced FTP processing

  Rev 5 -  23/Aug/04   -Added Scoresheet FTP

  Rev 4 -  27/Jul/04   -Fixed Flight Edit Dialog... some updates were lost.

  Rev 3 -  15/Jul/04   -Implemented rule 10.8.2.1.to allow for out the top starts & nick start cylinders
  
  Rev 2 -  06/May/04   -Fixed some bugs with Sought turnpoint and Flight Edit.

  Rev 1 -  06/May/04   -Cleaned up automatic MAT turnpoint selection.

  Rev 5 -  18/June/03  -Fixed situation where incorrect TAT fix is found.
						Fixed incorrect computation of out the top start times.

  Rev 4 -  30/May/03  -	Added warning in Edit... flight logs.
						Fixed crash in flight log import when no task was defined.

						1) Fixed problem where inner turnpoint and outer turnpoint radii were taken as the same unless explicitly set.  I think this was one of Marc's issues
 						2) New algorithm to  to select/reject turnpoints barley nicked during a MAT. Basically if the guy only entered the outer radius, I check to see if including that point will increase the total distance more than a mile.  If not, I discard that turnpoint.
 						3) User interface changes on the Edit Fix dialog to address some "Human Factors"
 						4) Fixed problems where missed turnpoints weren't properly warned.
 						5) Added warning if constructive landout was less than SMTD/2
 						6) Added warning if final leg in a MAT task is less than 5 miles.
 						7) Fixed problem in TAT where pilot entered turnpoint, thermaled out, then reentered further but was scored only on first entry.
 						8) Fixed the dropdown box in the Edit Fix dialog that doesn't handle more than 9 turnpoints.

							
  Rev 3 -  17/April/03  -	Fixed Some Flight Analysis problems.  Cleaned up printouts

  Rev 2 -  2/April/03  -	Changed flight files to be .zip and xml based.
							Fixed furthest progress for MAT tasks.

  Rev 1 -  16/March/03  -	Fixed scoring problems at the Seniors

  Rev 0 -  1/March/03  -	Initial 2003 release




  Rev 0 -  8/March/02  -	Initial 2002 release

  Rev 1 -  9/March/02  -	Added context menu in score list

  Rev 2 -  10/March/02 -	* Added SSA# to Cum Scoresheet export.
							* Fixed Class name not being printed on top of scoresheet.
							* Added Back & Forward Buttons

  Rev 3 -  11/March/02 -	* Fixed start altitude always 5000 in flight analysis.

  Rev 4 -  12/March/02 -	* Fixed furthest progress determination when 1st turnpoint not acheived
							* Automatically use furthest progress when computing distance for non finishers on Turn area tasks.

  Rev 5 -  18/March/02 -	* Reworked Time entry form.
							* Got rid of FDI

  Rev 6 -  19/March/02 -	* Added Barogram view to flight log display.
							* Added SUA checking
							* Reworked duplicate "Class Class" label after some classes

  Rev 7 -  31/March/02 -	* Removed Task Planner.
							* Remove STI entry and checking.
							* Removed check for roll times and start times for distance only
							* Added document set modified flag for flight analysis
							* Fixed bug where not all directories were searched
							* Fixed bug where view flight analysis tabs were garbage
							* Fixed bug where acheived turnpoints were not displayed in stored landing cards.
							* Made sorted columns persistant.
							* Added warning if FPF progress was further than landing location.
							* Reset Motorized flag when contestant was entered.

  Rev 8 -  9/April/02 -		* Added Handicap lookup.

  Rev 9 -					* Removed Guests from Takeoff Grid.
							* Saved Handicap calculation data (Weights, span, winglets)

  Rev 10 -					* Added paranoia check to stop checking for a TAT most favoriable fix after the contestant finished.
							* Fixed problem where handicap entry was not automatically
							  entered from glider id
							* Fixed problem where CAI file was not automatically converted to IGC
							* Added warning/info messages to landing card entry.

  Rev 11 -					* Fixed date rollover problem with IGC files.  The symptom of this was that the logs would not be found.

  Rev 12 -					* Fixed problem where TAT analysis selected first fix in turn area when a valid finish had not been made.
							* Fixed problem where contestants with el-cheapo loggers which don't record altitude were not given good finishes.

  Rev 13 -					* Fixed Crash analyzing flight log when the guy never took off.

  Rev 14 -					* Upgraded to automatically look at conus.sua airspace file.

  Rev 15 -					* Updated 2002 1-26 rules.

  Rev 16 -					* Fixed TAT bug with repetitive turnpoints

  Rev 17 -					* Fixed SUA reader not recognizing UNLTD top altitudes.

*/
#endif // !defined(WSVERSION_H_INCLUDED_)