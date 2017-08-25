<?php
class NDB_BVL_Instrument_svf extends NDB_BVL_Instrument
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
        $this->testName = "svf";           // test_names.Test_name
        $this->table = 'svf';             // name of table containing


        // data keyed by commentID
        $this->commentID = $commentID;

        //$this->_requiredElements = array('Date_taken', 'Examiner');
        
        $this->dateTimeFields=array("Date_taken", "clinical_1_date", "moca_date", "diagnosis_date", "onset_date");
        $this->_selectMultipleElements = array("region_of_onset");
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

    // Header & Defining Variables


        $hr_header = "<hr style='height:4px; border:none; color:rgb(58,106,117); margin-top: 0px; background-color:rgb(58,106,117)'; align='left'; width='1000px'>";
        $hr_section = "<hr style='height:1px; border:none; color:rgb(58,106,117); margin-top: 0px; background-color:rgba(58,106,117,0.5)'; align='left'; width='1000px'>";

        //Defines the call back function for HTML Quickform to use when validating the form.

            $this->form->addFormRule(array(&$this,'XINValidate'));

        // display test name

            $this->form->addElement('header', 'instrument_title', "S<span style='font-size: 26px'>CREEN </span>V<span style='font-size: 26px'>ISIT</span> F<span style='font-size: 26px'>ORM</span>");
            $this->form->addElement('static', null, $hr_header);

        // automatically adds examiner & date of administration

            $this->_addMetadataFields();
            $this->form->addElement('static', null, $hr_header);

        // Options

            $yes_no_option= array(NULL=>"", "yes"=>"Yes", "no"=>"No", "not_answered"=>"Not Answered");

            // Demographic options

                $gender_options= array(NULL=>"", "male"=>"Male","female"=>"Female", "not_answered"=>"Not Answered");
                $hand_preference_options= array(NULL=>"", "right"=>"Right","left"=>"Left", "ambidextrous"=>"Ambidextrous","not_answered"=>"Not Answered");
                $preferred_language_options= array(NULL=>"", "english"=>"English","french"=>"French", "other"=>"Other","not_answered"=>"Not Answered");
                $education_options = array(NULL=>"", "elementary"=>"Elementary", "high_school"=>"High School","post_secondary"=>"Post-Secondary","declined"=>"Declined","not_answered"=>"Not Answered");
                $communication_options = array(NULL=>"","gestures"=>"Gestures","writing"=>"Writing","technology"=>"Technology","other"=>"Other");
                $clinical_options = array(NULL=>"","yes"=>"Yes","past"=>"Past","unknown"=>"Unknown","never"=>"Never");

            // Onset information options

                $onset_options = array(NULL=>"","bulbar_speech"=>"Bulbar Speech","bulbar_swallowing"=>"Bulbar Swallowing","upper_extremity"=>"Upper Extremity","lower_extremity"=>"Lower Extremity","repiratory"=>"Respiratory","ftd_cognitive"=>"FTD/Cognitive","not_available"=>"N/A");
                $first_motor_onset_options = array(NULL=>"","right"=>"Right","left"=>"Left","both"=>"Both","unknown"=>"Unknown");
                $first_symptom_options = array(NULL=>"","weakness"=>"Weakness","fasciculations"=>"Fasciculations","atrophy"=>"Atrophy","cramping"=>"Cramping","stiffness"=>"Stiffness","other"=>"Other");

    // Demographics

        // Header for form

            $this->form->addElement('static', null, "D<span style='font-size: 12px'>EMOGRAPHICS</span>");

        // Horizontal Line Break
 
            $this->form->addElement('static', null, "<br>".$hr_section);       

        // Age

            // Entry for Age

                $this->form->addElement('text', 'age', $this->indent.'Age at time of screen visit:');
                $this->form->addRule('age','Numerical value required.', 'numeric');
              
        // Horizontal Line Break

             $this->form->addElement('static', null, $hr_section);

        // Gender

            // Entry for Gender

                $this->form->addElement('select', 'gender', $this->indent.'Gender:', $gender_options);
              
        // Horizontal Line Break

             $this->form->addElement('static', null, $hr_section);
      
        // Hand Preference

            $this->form->addElement('select', 'hand_preference', $this->indent.'Hand Preference:', $hand_preference_options);

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);
       
        // Preferred Language
        $this->form->addElement('select', 'preferred_language', $this->indent.'Preferred Lanuage:', $preferred_language_options);

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);
       
        // Highest Level of education
        $this->form->addElement('select', 'highest_education', $this->indent.'Highest Level of Education:', $education_options);

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);
       
        // Years of education beginning at Grade 1
        $this->form->addElement('numeric','e_years', $this->indent.'Years of Education Beginning at Grade 1: ');
        $this->form->addRule('e_years','Numerical value required.', 'numeric');

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);
       
        // Age participant began learning English
        $this->form->addElement('numeric','a_english', $this->indent."Age Participant Began Learning English <span style='font-weight:normal'><i>(Enter '0' if at birth): </i></span>");
        $this->form->addRule('a_english','Numerical value required.', 'numeric');

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);
       
        // Is participant able to read English?
        $this->form->addElement('select', 'read_english', $this->indent.'Is Participant Able to Read English:', $yes_no_option);

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);
       
        // Birthplace (city,country)
        $this->form->addElement('text','birth_place', $this->indent."Birthplace <span style='font-weight:normal'><i>(City,Country):</i></span>");

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);
       
        // raised (city,country)
        $this->form->addElement('text','raised_place', $this->indent."Where was participant primarily raised <span style='font-weight:normal'><i>(City,Country)?</i></span>");

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);
       
        // Does participant communicate by speaking?
        $this->form->addElement('select', 'communication', $this->indent.'Does participant communicate by speaking?', $yes_no_option);
        $this->form->addElement('select', 'communication_type', $this->indent.$this->indent."<span style='font-weight:normal'><i>If no, what is used to communicate?</i></span>", $communication_options);
        $this->form->addElement('textarea','c_list', $this->indent.$this->indent."<span style='font-weight:normal'><i>If other, list:</i></span>");
        $this->XINRegisterRule("communication_type", array("communication{@}=={@}no"), "A type of communication must be specified.");
        $this->XINRegisterRule("c_list", array("communication_type{@}=={@}other"), "Other communication(s) must be specified.");

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);
       
        // Experienced changes
        $this->form->addElement('select', 'e_changes', $this->indent.'Has participant experienced changes in their speech in the past 2 years?', $yes_no_option);

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);
       
        // Clinical Trials
        $this->form->addElement('select', 'clinical_trials', $this->indent.'Is participant currently in a clinical trial?', $clinical_options);
        $this->form->addElement('text','clinical_1', $this->indent.$this->indent."<span style='font-weight:normal'><i>If yes, name of clinical trial:</i></span>");
        $this->form->addElement('date', 'clinical_1_date', $this->indent.$this->indent."<span style='font-weight:normal'><i>Date of clinical trial mentioned above:</i></span>");
        $this->XINRegisterRule("clinical_1", array("clinical_trials{@}=={@}yes"), "A clinical trial name is required.");
        $this->XINRegisterRule("clinical_1_date", array("clinical_trials{@}=={@}yes"), "A clinical trial date is required.");

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_header);


    // Onset information (patients only)

        // Continue only if subject is a patient

        if ($this->getSubprojectID()==2) {  

            
            // Header for form

            $this->form->addElement('static', null, "O<span style='font-size: 12px'>NSET INFORMATION</span>");

             // Horizontal Line Break
 
            $this->form->addElement('static', null, "<br>".$hr_header);       

            
            // Symptom onset date
            $this->addBasicDate('onset_date', $this->indent.'Symptom onset date:');
            
            // Horizontal Line Break
 
            $this->form->addElement('static', null, "<br>".$hr_section);  

            // Diagnosis date
            $this->addBasicDate('diagnosis_date', $this->indent.'Diagnosis date:');
            
            // Horizontal Line Break
 
            $this->form->addElement('static', null, "<br>".$hr_section);  

            // Region of onset
            $this->form->addElement('select', 'region_of_onset', $this->indent."Region of onset <span style='font-weight:normal'><i>(press 'ctrl' to select more than one option):</i></span>", $onset_options, array('multiple' => 'multiple'));
            
            // Horizontal Line Break
 
            $this->form->addElement('static', null, "<br>".$hr_section);  

            // Side of first motor symptom onset
            $this->form->addElement('select', 'first_motor_onset', $this->indent.'Side of first motor symptom onset:', $first_motor_onset_options);
            
            // Horizontal Line Break
 
            $this->form->addElement('static', null, "<br>".$hr_section);  

            // First symptom (if limb onset)
            $this->form->addElement('select', 'first_symptom', $this->indent."First symptom <span style='font-weight:normal'><i>(if limb onset):</i></span>", $first_symptom_options);
            $this->form->addElement('textarea','s_list', $this->indent.$this->indent."<span style='font-weight:normal'><i>If other, list:</i></span>");
            $this->XINRegisterRule("s_list", array("first_symptom{@}=={@}other"), "Please list the first symptom(s).");

            // Horizontal Line Break
 
            $this->form->addElement('static', null, "<br>".$hr_section);  

            // Notes
            $this->form->addElement('textarea','notes', $this->indent.'Provide any additional notes:');
            $this->XINRegisterRule("notes",array("notes{@}=={@}NEVER_REQUIRED"),"Not required");

            // Horizontal Line Break
 
            $this->form->addElement('static', null, "<br>".$hr_header);  
        }

    // Neuropsychometric Evaluations Form

        // Header for form

            $this->form->addElement('static', null, "N<span style='font-size: 12px'>EUROPSYCHOMETRIC EVALUATIONS</span>");

            // Horizontal Line Break
 
                $this->form->addElement('static', null, "<br>".$hr_section);  

             // Is MoCA done?
                 $this->form->addElement('select', 'moca', $this->indent."Montreal Cognitive Assessment <span style='font-weight:normal'><i>(MoCA):</i></span>", array(NULL=>"","done"=>"Done","not_done"=>"Not Done")); 

            // Horizontal Line Break
 
                $this->form->addElement('static', null, $hr_header);  

               
        // Date
        $this->form->addElement('date', 'moca_date', $this->indent.'Date:');
        $this->XINRegisterRule("moca_date", array("moca{@}=={@}done"), "A diagnosis date must be provided.");

        // Horizontal Line Break
 
            $this->form->addElement('static', null, $hr_section);

        // Type
        $this->form->addElement('select', 'type', $this->indent.'Type:', array(NULL=>"","spoken"=>"Spoken","written"=>"Written"));
        $this->XINRegisterRule("type", array("moca{@}=={@}done"), "A type must be provided.");

        // Horizontal Line Break
 
            $this->form->addElement('static', null, $hr_section);

        // Visuospatial/Executive Score
        $this->form->addElement('numeric','visuospatial', $this->indent."Visuospatial/Executive Score <span style='font-weight:normal'><i>(Out of 5):</i></span>");
        $this->form->addElement('select','modification', $this->indent . $this->indent."<span style='font-weight:normal'><i>Modifications drawings used?</i></span>", $yes_no_option);
        $this->XINRegisterRule("modification", array("moca{@}=={@}done"), "An answer must be provided.");
        $this->form->addRule('visuospatial','Numerical value required.', 'numeric',0,5);
        $this->XINRegisterRule("visuospatial", array("moca{@}=={@}done"), "A score must be provided.");

        // Horizontal Line Break
 
            $this->form->addElement('static', null, $hr_section);

        // Naming score
        $this->form->addElement('numeric','naming_score', $this->indent."Naming Score <span style='font-weight:normal'><i>(Out of 3):</i></span>");
        $this->form->addRule('naming_score','Numerical value required.', 'numeric', 0, 3);
        $this->XINRegisterRule("naming_score", array("moca{@}=={@}done"), "A score must be provided.");

        // Horizontal Line Break
 
            $this->form->addElement('static', null, $hr_section);

        // Attention score
        $this->form->addElement('numeric','attention_score', $this->indent."Attention Score <span style='font-weight:normal'><i>(Out of 6):</i></span>");
        $this->form->addRule('attention_score','Numerical value required.', 'numeric',0,6);
        $this->XINRegisterRule("attention_score", array("moca{@}=={@}done"), "A score must be provided.");

        // Horizontal Line Break
 
            $this->form->addElement('static', null, $hr_section);


        // Language score
        $this->form->addElement('numeric','language_score', $this->indent."Language Score <span style='font-weight:normal'><i>(Out of 3):</i></span>");
        $this->form->addRule('language_score','Numerical value required.', 'numeric',0,3);
        $this->XINRegisterRule("language_score", array("moca{@}=={@}done"), "A score must be provided.");

        // Horizontal Line Break
 
            $this->form->addElement('static', null, $hr_section);


        // Abstraction score
        $this->form->addElement('numeric','abstraction_score', $this->indent."Abstraction Score <span style='font-weight:normal'><i>(Out of 2):</i></span>");
        $this->form->addRule('abstraction_score','Numerical value required.', 'numeric', 0,2);
        $this->XINRegisterRule("abstraction_score", array("moca{@}=={@}done"), "A score must be provided.");

        // Horizontal Line Break
 
            $this->form->addElement('static', null, $hr_section);


        // Delayed recall score
        $this->form->addElement('numeric','delayed_recall_score', $this->indent."Delayed Recall Score <span style='font-weight:normal'><i>(Out of 5):</i></span>");
        $this->form->addRule('delayed_recall_score','Numerical value required.', 'numeric',0,5);
        $this->XINRegisterRule("delayed_recall_score", array("moca{@}=={@}done"), "A score must be provided.");

        // Horizontal Line Break
 
            $this->form->addElement('static', null, $hr_section);

        // Orientation score
        $this->form->addElement('numeric','orientation_score', $this->indent."Orientation Score <span style='font-weight:normal'><i>(Out of 6):</i></span>");
        $this->form->addRule('orientation_score','Numerical value required.', 'numeric',0,6);
        $this->XINRegisterRule("orientation_score", array("moca{@}=={@}done"), "A score must be provided.");

        // Horizontal Line Break
 
            $this->form->addElement('static', null, $hr_section);


        // Total Score
        $this->form->addElement('numeric','total_score', $this->indent."Total Score <span style='font-weight:normal'><i>(+1 point if years of education is <12):</i></span>");
        $this->form->addRule('total_score','Numerical value required.', 'numeric',0,30);
        $this->XINRegisterRule("total_score", array("moca{@}=={@}done"), "A score must be provided.");
   
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
        $this->addBasicDate('Date_taken', 'Date of Administration', $dateOptions);
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
        $this->addSelect('Examiner', 'Examiner', $examiners);
        $this->addRule(
            'Date_taken',
            'Date of Administration is required',
            'required'
        );
        $this->addRule('Examiner', 'Examiner is required', 'required');
    
    }
}
?>
