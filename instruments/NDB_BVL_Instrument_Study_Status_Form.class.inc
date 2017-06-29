<?php
/**
* The Study Status Form Instrument
*
* PHP Version 7
*
* @category Behavioral_Instruments
* @package  Study_Status_Form
* @author   Muhammad <mukhan@ualberta.ca>
* @license  http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
* @link     https://www.github.com/aces/Loris/
*/

/**
 * Implements the candidate_list menu
 *
* @category Main
* @package  Candidate_List
* @author   Loris Team <loris.mni@bic.mni.mcgill.ca>
* @license  http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
* @link     https://www.github.com/aces/Loris/
*/
class NDB_BVL_Instrument_Study_Status_Form extends NDB_BVL_Instrument
{
    var $ValidityEnabled  = false;
    var $ValidityRequired = false;

    /**
    * Sets up basic data, such as the HTML_Quickform object, and so on.
    *
    * @param  string $commentID the CommentID identifying the data to load
    * @param  string $page      if a multipage form, the page to show
    * @return void
    * @access public
    */
    function setup($commentID, $page)
    {

        $this->formType ="XIN";
        $this->form     = new LorisForm('test_form');
        $this->page     = $page;            // page label (number or
                                        // string - used by
                                        // user-defined child classes)

        // set the object properties
        $this->testName = "Study_Status_Form";           // test_names.Test_name
        $this->table    = 'Study_Status_Form';             // name of table containing

        // data keyed by commentID
        $this->commentID = $commentID;

        $this->_requiredElements = array(
                                    'Date_taken',
                                    'Examiner',
                                    'form_type',
                                   );

        $this->dateTimeFields =array(
                                "Date_taken",
                                'status_date',
                                'deceased_date',
                                'peg_insertion_date',
                                'bipap_start_date',
                                'pav_date',
                                'tracheostomy_date',
                               );

        // setup the form
        $this->_setupForm();
    }



    /**
    * method to build the HTML_Quickform object into a paged form
    *
    * @return void
    * @access private
    */
    function _setupForm()
    {

        // Header Options

            $hr_header  = "<hr style='height:2px; border:none; color:rgb(58,106,117); margin-top: 0px; background-color:rgb(58,106,117)'; align='left'; width='850px'>";
            $hr_section = "<hr style='height:1px; border:none; color:rgb(58,106,117); margin-top: 0px; background-color:rgba(58,106,117,0.5)'; align='left'; width='850px'>";

        //Defines the call back function for HTML Quickform to use when validating the form.

            $this->form->addFormRule(array(&$this, 'XINValidate'));

        // Ensures that all the dependent date fields are only filled out if the referencing field is set to 'Yes'.

            $this->form->addFormRule(array(&$this, 'validateDateFields'));

        // display test name

            $this->form->addElement('header', 'instrument_title', "S<span style='font-size: 26px'>TUDY </span>S<span style='font-size: 26px'>TATUS</span> F<span style='font-size: 26px'>ORM</span>");
            $this->form->addElement('static', null, $hr_header);

        // automatically adds examiner & date of administration

            $this->_addMetadataFields();
            $this->form->addElement('static', null, $hr_section);

        // Options

            $participant_options = array(
                                    null                => "",
                                    "Active"            => "Active",
                                    "Withdrawn"         => "Withdrawn",
                                    "Lost-to Follow-up" => "Lost-to Follow-up",
                                    "Deceased"          => "Deceased",
                                   );
            if ($this->getVisitLabel() =='V0') {
                $normal_option = array(
                                  null      => "",
                                  "Initial" => "Initial",
                                 );
            } else {
                $normal_option = array(
                                  null        => "",
                                  "Update"    => "Update",
                                  "No Change" => "No Change",
                                 );
            }

            $yes_no_option = array(
                              null  => "",
                              "Yes" => "Yes",
                              "No"  => "No",
                             );

            // Form Type

            $this->form->addElement('select', 'form_type', $this->indent."Form Type?", $normal_option);
            $this->form->addElement('static', null, $hr_section);

            // Checking to see if the same form from the previous visit has been completed (all adminisitered, and completed status) only if it is not V0 (Screening)

            if ($this->getVisitLabel() !='V0') {

                $this->form->addFormRule(array(&$this, 'validatePreviousFormCompletion'));

            }

            // Participant Status

            // Participant Status Field

            $this->form->addElement('select', 'status_participant', $this->indent.'Participant Status:', $participant_options);

            // Participant Status required if Form Type is Initial

                    $this->XINRegisterRule("status_participant", array("form_type{@}=={@}Initial"), "Required.");

            // Status date

            $this->form->addElement('date', 'status_date', $this->indent.$this->indent."<span style='font-weight:normal'><i>Consent/Withdrawal/Lost-to Follow-up/Deceased Date:</span></i>");

            // Required If an option from Participant Status field is selected

                    $this->XINRegisterRule("status_date", array("status_participant{@}=={@}Active|Withdrawn|Lost-to Follow-up|Deceased"), "Please provide the consent/withdraw/last contact/deceased date!");

            // Previously I had a date field for each status, however, I have made one date field that would accomodate for all of the statuses.

            // Consent Date

                //$this->form->addElement('date', 'consent_date', $this->indent.$this->indent."<span style='font-weight:normal'><i>If active, date of consent:</span></i>");

                // Required If Participant Status is Active

                    //$this->XINRegisterRule("consent_date", array("status_participant{@}=={@}Active"), "Please provide the consent date if the participant is active!");

            // Withdrawn Date

                //$this->form->addElement('date', 'withdrawn_date', $this->indent.$this->indent."<span style='font-weight:normal'><i>If withdrawn, date of withdrawal:</i></span>");

                // Required if Participant Status is Withdrawn

                    //$this->XINRegisterRule("withdrawn_date", array("status_participant{@}=={@}Withdrawn"), "Please provide the withdrawn date if the participant withdrew from the study!");

               // Lost to follow-up Date

                //$this->form->addElement('date', 'lost_to_followup_date', $this->indent.$this->indent."<span style='font-weight:normal'><i>If lost-to follow-up, date of last contact:</i></span>");

                // Required if Participant Status is Lost-to Follow-up

                    //$this->XINRegisterRule("lost_to_followup_date", array("status_participant{@}=={@}Lost-to Follow-up"), "Please provide the date of last contact!");

            // Deceased Date

                //$this->form->addElement('date', 'deceased_date', $this->indent.$this->indent."<span style='font-weight:normal'><i>If deceased, date of death:</i></span>");

                // Required if Participant Status is Deceased

                    //$this->XINRegisterRule("deceased_date", array("status_participant{@}=={@}Deceased"), "Please provide the date of death!");

            $this->form->addElement('static', null, $hr_section."<br>");

            // Continue only if subject is a patient
            if ($this->getSubprojectID()==2) {

                // PEG Insertion

                // PEG Insertion Field

                    $this->form->addElement('select', 'peg_insertion', $this->indent.'PEG insertion:', $yes_no_option);

                    // PEG Insertion required if Form Type is Initial

                           $this->XINRegisterRule("peg_insertion", array("form_type{@}=={@}Initial"), "Please provide if the patient had a PEG insertion!");

                   // PEG Insertion Date Field

                    $this->form->addElement('date', 'peg_insertion_date', $this->indent.$this->indent."<span style='font-weight:normal'><i>If yes, PEG insertion date:</i></span>");

                    // PEG Insertion Date Field is Required if PEG Insertion is Yes

                        $this->XINRegisterRule("peg_insertion_date", array("peg_insertion{@}=={@}Yes"), "Please provide the PEG insertion date!");

                $this->form->addElement('static', null, $hr_section);

                // BiPAP start

                    // BiPAP Start Field

                        $this->form->addElement('select', 'bipap_start', $this->indent.'BiPAP Start:', $yes_no_option);

                        // BiPAP Start required if Form Type is Initial

                            $this->XINRegisterRule("bipap_start", array("form_type{@}=={@}Initial"), "Please provide if the patient started BiPAP!");

                    // BiPAP Start Date

                        $this->form->addElement('date', 'bipap_start_date', $this->indent.$this->indent."<span style='font-weight:normal'><i>If yes, BiPAP start date:</i></span>");

                        // BiPAP Start Date required if BiPAP Start is Yes

                            $this->XINRegisterRule("bipap_start_date", array("bipap_start{@}=={@}Yes"), "Please provide the BiPAP start date!");

                $this->form->addElement('static', null, $hr_section);

                // PAV GitTest

                // PAV Field

                    $this->form->addElement('select', 'pav', $this->indent.'PAV > 22hrs/day:', $yes_no_option);

                    // PAV Field required if Form Type is Initial

                           $this->XINRegisterRule("pav", array("form_type{@}=={@}Initial"), "Please provide an answer to the above field!");

                   // PAV Date Field

                    $this->form->addElement('date', 'pav_date', $this->indent.$this->indent. "<span style='font-weight:normal'><i>If yes, date:</i></span>");

                    // PAV Date Field required if PAV is Yes

                        $this->XINRegisterRule("pav_date", array("pav{@}=={@}Yes"), "Please provide the date!");

                $this->form->addElement('static', null, $hr_section);

                // Tracheostomy

                // Tracheostomy Field

                    $this->form->addElement('select', 'tracheostomy', $this->indent.'Tracheostomy:', $yes_no_option);

                    // Tracheostomy Field required if Form Type is Initial

                        $this->XINRegisterRule("tracheostomy", array("form_type{@}=={@}Initial"), "Please provide an answer to the above field!");

                // Tracheostomy Date Field

                    $this->form->addElement('date', 'tracheostomy_date', $this->indent.$this->indent."<span style='font-weight:normal'><i>If yes, date:</i></span>");

                    // Tracheostomy Date Field required is Tracheostomy is Yes

                        $this->XINRegisterRule("tracheostomy_date", array("tracheostomy{@}=={@}Yes"), "Please provide the date!");

                $this->form->addElement('static', null, $hr_section);

            }

            // Notes

            $this->form->addElement('textarea', 'notes', $this->indent.'Notes:');

            // Notes Field not required

                $this->XINRegisterRule("notes", array("notes{@}=={@}NEVER_REQUIRED"), "Not required");

    } // End Setup form
    function validateDateFields($values)
    {

        $checkDependentFields =array(
                                'deceased',
                                'peg_insertion',
                                'bipap_start',
                                'pav',
                                'tracheostomy',
                               );
        $errors = array();

        foreach ($checkDependentFields as $field) {

            if ($values[$field]=='No'&& !empty($values[$field.'_date'])) {
                $errors[$field.'_date'] = "Please remove the date!";
            }
        }

        return $errors;

    }
    /**
    * Used to check if user has access to the instrument
    *
    * @return bool
    */
    function _hasAccess()
    {
        // create user object
        $user =& User::singleton();

        $timePoint =& TimePoint::singleton($_REQUEST['sessionID']);
        $candID    = $timePoint->getCandID();

        $candidate =& Candidate::singleton($candID);

        // check user permissions
        return ($user->hasPermission('access_all_profiles') || $user->getData('CenterID') == $candidate->getData('CenterID') || $user->getData('CenterID') == $timePoint->getData('CenterID'));
    }

    /**
 * Used to check if the previous Form has been created and completed.
*/
    function validatePreviousFormCompletion($values)
    {

        $db     =& Database::singleton();
        $errors = array();
        $previousCommentID = $this->getPreviousCommentID();
        $query  = "SELECT Data_entry, Administration from flag where CommentID=:previousCommentID";
        $result = $db->pselectRow($query, array('previousCommentID' => $previousCommentID));
        if($result['Data_entry']!='Complete' || $result['Administration']!='All') {
            $errors['form_type'] = "The ".$this->getPreviousVisitLabel()." ".str_replace('_', ' ', $this->testName)." must be set to <br>'All' for Administration and 'Complete' for Data Entry.";
        }

        return $errors;
    }

    /**
 * Used to retreive the current visit label
*/
    function getVisitLabel()
    {
        $timePoint  =& TimePoint::singleton($_REQUEST['sessionID']);
        $visitLabel = $timePoint->getVisitLabel();
        return $visitLabel;
    }

    /**
 * Used to check the projectID of the candidate (control or patient)
*/
    function getSubprojectID()
    {

        $timePoint    =& TimePoint::singleton($_REQUEST['sessionID']);
        $SubprojectID = $timePoint->getSubprojectID();
        return $SubprojectID;
    }

    /**
 * Used to get the previous visit label
*/
    function getPreviousVisitLabel()
    {
        $currentVisitLabelNumber = $this->getVisitLabel()[1];
        $previousVisitLabel      ='V'.($currentVisitLabelNumber-1);
        return $previousVisitLabel;
    }

    /**
 * Returns the previous commentID
*/
    function getPreviousCommentID()
    {
        $db     =& Database::singleton();
        $candID = $_GET['candID'];
        $previousVisitLabel = $this->getPreviousVisitLabel();
        $query  = "SELECT CommentID from session s, flag f where s.ID=f.SessionID AND Test_name=:test_name AND CommentID LIKE CONCAT(:candID,'%') AND Visit_label=:previousVisitLabel";
        $result = $db->pselectRow($query, array('test_name' => $this->testName, 'candID' => $candID, 'previousVisitLabel' => $previousVisitLabel));
        return $result['CommentID'];
    }

    /**
 * Study Status Form Field Retreival @return void
*/
    function score()
    {
        // Remove the comment tags if you would like to retreive fields only after the current form is completed.

            //if ($this->_determineDataEntryCompletionStatus() == "Incomplete") {
            //    return;
            //}

        // Connect to the LORIS Database
        $db =& Database::singleton();

        // The query to retreive all the fields and their respective values of this form.
        $query_current = "SELECT * FROM " . $this->table .
            " WHERE CommentID='" . $this->getCommentID() . "'";

        // Retrieval of fields and their values
        $record_current = $db->pselectRow($query_current, null);

        // The form_type of the current form. Checks to see if it is initial, update, or no change.
        $form_type = $record_current['form_type'];

        // If the form is not initial and the visit label is not V0 then retreive all the field values of the same form from the previous visit.
        if ($form_type != 'Initial' && $this->getVisitLabel()!= 'V0') {

            // The function determines the CommentID of the same form from the previous visit.
            $previousCommentID = $this->getPreviousCommentID();

            // Generating a query to retreive fields of the same form from the previous visit
            $query_previous = "SELECT * FROM ".$this->testName." WHERE CommentID=:previousCommentID";

            // All field values from the previous visit are stored in this variable.
            $updateTo = $db->pselectRow($query_previous, array('previousCommentID' => $previousCommentID));

            // This removes the fields that will or cannot be updated from the current form.
            unset($record_current['CommentID'], $record_current['UserID'], $record_current['Examiner'],$record_current['Testdate'],$record_current['Data_entry_completion_status'],$record_current['Date_taken'],$record_current['form_type']);

            // The updateTo variable contains all the values for fields from the previous form that will be used to update the current form.
            // There are a few fields that cannot be updated, as they are particular to the current visit (ex. primary keys).
            // So to avoid issues with data integrity, these fields are removed and only hard copy form fields will be updated..
            unset($updateTo['CommentID'], $updateTo['UserID'], $updateTo['Examiner'],$updateTo['Testdate'],$updateTo['Data_entry_completion_status'],$updateTo['Date_taken'],$updateTo['form_type']);

            // If the form type is Update, it checks for all the fields that are not null and removes them from the update query. The rationale behind this is that all non-empty fields are the updated fields, and we do not want to overwrite them with the previous fields.
            if ($form_type=='Update') {
                foreach ($record_current as $key => $val) {
                    if (!is_null($record_current[$key])) {
                        unset($updateTo[$key]);
                    }
                }
            }

            // After removing any additional updated fields, the values of the fields are saved to the current form fields.
            // save the scores
            $db->update($this->table, $updateTo, array('CommentID' => $this->getCommentID()));
        }

    }
    /**
    * Adds metadata fields (such as Examiner and Date_taken) to the
    * current form
    *
    * @return void
    * @access private
    */
    function _addMetadataFields()
    {
        $factory           = \NDB_Factory::singleton();
        $config            = $factory->config();
        $dateOptions       = array(
                              'language'         => 'en',
                              'format'           => 'YMd',
                              'minYear'          => $config->getSetting('startYear'),
                              'maxYear'          => $config->getSetting('endYear'),
                              'addEmptyOption'   => true,
                              'emptyOptionValue' => null,
                             );
        $this->dateOptions = $dateOptions;
        $this->addBasicDate('Date_taken', $this->indent.'Date of Administration', $dateOptions);
        //if (strrpos($this->testName, '_proband') === false) {
        //    $this->addScoreColumn(
        //        'Candidate_Age',
        //        'Candidate Age (Months)',
        //        'Please enter date of administration and press Save'
        //    );
        //    $this->addScoreColumn(
        //        'Window_Difference',
        //        'Window Difference (+/- Days)',
        //        'Please enter date of administration and press Save'
        //    );
        //}
        $examiners = $this->_getExaminerNames();
        $this->addSelect('Examiner', $this->indent.'Examiner', $examiners);
        $this->addRule(
            'Date_taken',
            'Date of Administration is required',
            'required'
        );
        $this->addRule('Examiner', 'Examiner is required', 'required');

    }
}
?>
