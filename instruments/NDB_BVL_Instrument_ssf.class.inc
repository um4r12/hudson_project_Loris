<?php
class NDB_BVL_Instrument_ssf extends NDB_BVL_Instrument
{
    var $ValidityEnabled = false;
    var $ValidityRequired = false;

    /**
    * sets up basic data, such as the HTML_Quickform object, and so on.
    *
    * @param string $commentID  the CommentID identifying the data to load
    * @param string $page       if a multipage form, the page to show
    * @return void
    * @access public
    */
    function setup($commentID, $page){
        $this->formType="XIN";
        $this->form = new LorisForm('test_form');
        $this->page = $page;            // page label (number or
                                        // string - used by
                                        // user-defined child classes)

        // set the object properties
        $this->testName = "ssf";           // test_names.Test_name
        $this->table = 'ssf';             // name of table containing


        // data keyed by commentID
        $this->commentID = $commentID;

        //$this->_requiredElements = array('Date_taken', 'Examiner', 'form_type');
        
        $this->dateTimeFields=array("Date_taken", "consent_date", 'withdrawn_date', 'lost_to_followup_date', 'deceased_date', 'peg_insertion_date', 'bipap_start_date', 'pav_date', 'tracheostomy_date');
        
        // setup the form
        $this->_setupForm();
    }


    // Used to check if user has access to the instrument
    
    function _hasAccess()
    {
        // create user object
        $user =& User::singleton();

        $timePoint =& TimePoint::singleton($_REQUEST['sessionID']);
        $candID = $timePoint->getCandID();

        $candidate =& Candidate::singleton($candID);

        // check user permissions
        return ($user->hasPermission('access_all_profiles') || $user->getData('CenterID') == $candidate->getData('CenterID') || $user->getData('CenterID') == $timePoint->getData('CenterID'));
    }
    /** Used to check the projectID of the candidate (control or patient) */
    function getSubprojectID() {
        
        $db =& Database::singleton();
        $sessionID = $_GET[sessionID];
        $candID= $_GET[candID];
        if (empty(trim($sessionID))) {
            $SubprojectID=1;
            

        } else {
            $query = "SELECT SubprojectID from session where ID=:sessionID AND CandID=:candID";
            $SubprojectID = $db->pselectRow($query, array('candID'=>$candID,'sessionID'=>$sessionID))[SubprojectID];
            
        }
        
        return $SubprojectID;
        
    }
    /**
    * method to build the HTML_Quickform object into a paged form
    *
    * @return void
    * @access private
    */
    function _setupForm(){
        
        $hr_header = "<hr style='height:2px; border:none; color:rgb(58,106,117); margin-top: 0px; background-color:rgb(58,106,117)'; align='left'; width='850px'>";
        $hr_section = "<hr style='height:1px; border:none; color:rgb(58,106,117); margin-top: 0px; background-color:rgba(58,106,117,0.5)'; align='left'; width='850px'>";

        //Defines the call back function for HTML Quickform to use when validating the form.
        $this->form->addFormRule(array(&$this,'XINValidate'));

        // display test name
        $this->form->addElement('header', 'instrument_title', "S<span style='font-size: 26px'>TUDY </span>S<span style='font-size: 26px'>TATUS</span> F<span style='font-size: 26px'>ORM</span>");
        $this->form->addElement('static', null, $hr_header);

        // automatically adds examiner & date of administration
        $this->_addMetadataFields();        
        $this->form->addElement('static', null, $hr_section);
        
        $yes_no_option= array(NULL=>"", "yes"=>"Yes", "no"=>"No", "not_answered"=>"Not Answered");
        $normal_option= array(NULL=>"", "initial"=>"Initial","update"=>"Update", "no_change"=>"No Change", "not_answered"=>"Not Answered");
        $exclusionaryOrNot = array(NULL=>"", "exclusionary"=>"Exclusionary", "non_exclusionary"=>"Non-Exclusionary", "not_answered"=>"Not Answered");

        $this->form->addElement('select', 'form_type', $this->indent."Form Type?", $normal_option);
        $this->form->addElement('static', null, $hr_section);

        // 
        $this->XINRegisterRule("active", array("form_type{@}=={@}initial"), "Required.");
        $this->XINRegisterRule("withdrawn", array("form_type{@}=={@}initial"), "Required.");
        $this->XINRegisterRule("lost_to_followup", array("form_type{@}=={@}initial"), "Required.");
        $this->XINRegisterRule("peg_insertion", array("form_type{@}=={@}initial"), "Required.");
        $this->XINRegisterRule("bipap_start", array("form_type{@}=={@}initial"), "Required.");
        $this->XINRegisterRule("pav", array("form_type{@}=={@}initial"), "Required.");
        $this->XINRegisterRule("tracheostomy", array("form_type{@}=={@}initial"), "Required.");
        $this->XINRegisterRule("notes", array("form_type{@}=={@}update"), "Please provide the updates that were made.");


        // Active
        $this->form->addElement('select', 'active', $this->indent.'Active:', $yes_no_option);
        $this->form->addElement('date', 'consent_date', $this->indent.$this->indent."<span style='font-weight:normal'><i>If yes, consent date:</span></i>");
        $this->XINRegisterRule("consent_date", array("active{@}=={@}yes"), "Please provide the consent date!");
        $this->form->addElement('static', null, $hr_section);

        // Withdrawn
        $this->form->addElement('select', 'withdrawn', $this->indent.'Withdrawn:', $yes_no_option);
        $this->form->addElement('date', 'withdrawn_date', $this->indent.$this->indent."<span style='font-weight:normal'><i>If yes, withdrawn date:</i></span>");
        $this->XINRegisterRule("withdrawn_date", array("withdrawn{@}=={@}yes"), "Please provide the withdrawn date!");
        $this->form->addElement('static', null, $hr_section);
        // Lost to follow up
        $this->form->addElement('select', 'lost_to_followup', $this->indent.'Lost to Follow-up:', $yes_no_option);
        $this->form->addElement('date', 'lost_to_followup_date', $this->indent.$this->indent."<span style='font-weight:normal'><i>If yes, date of last contact:</i></span>");
        $this->XINRegisterRule("lost_to_followup_date", array("lost_to_followup{@}=={@}yes"), "Please provide the date of last contact!");
        $this->form->addElement('static', null, $hr_section."<br>");


        
        // Continue only if subject is a patient
        if ($this->getSubprojectID()==2) {


           
            $this->form->addElement('static', null, 'Fill out the next section only if subject is a patient.<br><br><br>');
            $this->form->addElement('static', null, $hr_section);

            // PEG Insertion
            $this->form->addElement('select', 'peg_insertion', $this->indent.'PEG insertion:', $yes_no_option);
            $this->XINRegisterRule("peg_insertion", array("form_type{@}=={@}initial"), "Please provide if the patient had a PEG insertion!");
            $this->form->addElement('date', 'peg_insertion_date', $this->indent.$this->indent."<span style='font-weight:normal'><i>If yes, PEG insertion date:</i></span>");
            $this->XINRegisterRule("peg_insertion_date", array("peg_insertion{@}=={@}yes"), "Please provide the PEG insertion date!");
            $this->form->addElement('static', null, $hr_section);

            // BiPAP start
            $this->form->addElement('select', 'bipap_start', $this->indent.'BiPAP Start:', $yes_no_option);
            $this->XINRegisterRule("bipap_start", array("form_type{@}=={@}initial"), "Please provide if the patient started BiPAP!");
            $this->form->addElement('date', 'bipap_start_date', $this->indent.$this->indent."<span style='font-weight:normal'><i>If yes, BiPAP start date:</i></span>");
            $this->XINRegisterRule("bipap_start_date", array("bipap_start{@}=={@}yes"), "Please provide the BiPAP start date!");
            $this->form->addElement('static', null, $hr_section);
            // PAV
            $this->form->addElement('select', 'pav', $this->indent.'PAV > 22hrs/day:', $yes_no_option);
            $this->XINRegisterRule("pav", array("form_type{@}=={@}initial"), "Please provide an answer to the above field!");
            $this->form->addElement('date', 'pav_date', $this->indent.$this->indent. "<span style='font-weight:normal'><i>If yes, date:</i></span>");
            $this->XINRegisterRule("pav_date", array("pav{@}=={@}yes"), "Please provide the date!");
            $this->form->addElement('static', null, $hr_section);

            // Tracheostomy
            $this->form->addElement('select', 'tracheostomy', $this->indent.'Tracheostomy:', $yes_no_option);
            $this->XINRegisterRule("tracheostomy", array("form_type{@}=={@}initial"), "Please provide an answer to the above field!");
            $this->form->addElement('date', 'tracheostomy_date', $this->indent.$this->indent."<span style='font-weight:normal'><i>If yes, date:</i></span>");
            $this->XINRegisterRule("tracheostomy_date", array("tracheostomy{@}=={@}yes"), "Please provide the date!");
            $this->form->addElement('static', null, $hr_section);
            
            // Deceased
            $this->form->addElement('select', 'deceased', $this->indent.'Deceased:', $yes_no_option);
            $this->XINRegisterRule("deceased", array("form_type{@}=={@}initial"), "Please provide an answer to the above field!");
            $this->form->addElement('date', 'deceased_date', $this->indent.$this->indent."<span style='font-weight:normal'><i>If yes, deceased date:</i></span>");
            $this->XINRegisterRule("deceased_date", array("deceased{@}=={@}yes"), "Please provide the date!");
            $this->form->addElement('static', null, $hr_section);
 
        }
        
        
        $this->form->addElement('textarea','notes', $this->indent.'Notes:');
    
    } // End Setup form

/**
    * adds metadata fields (such as Examiner and Date_taken) to the
    * current form
    *
    * @return void
    * @access private
    */
   function _addMetadataFields()
    {
        $factory = \NDB_Factory::singleton();
        $config  = $factory->config();
        $dateOptions = array(
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
