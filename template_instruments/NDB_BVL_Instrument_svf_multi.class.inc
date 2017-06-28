<?php
class NDB_BVL_Instrument_svf_multi extends NDB_BVL_Instrument
{
	var $ValidityEnabled = true;
    var $ValidityRequired = true;
    /**
     * Sample SQL statement for test_names table and instrument subtests table
     *
     * INSERT INTO test_names(Test_name, Full_name,Sub_group,isDirectEntry) VALUES ('<TEST_NAME>','<INSTRUMENT_TITLE>',1, false);
     * INSERT INTO instrument_subtests(Test_name, Subtest_name, Description, Order_number) VALUES('<TEST_NAME>', '<TEST_NAME>_page1', 'Page1', 1);
     */

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
        $this->testName = "svf_multi";           // test_names.Test_name
        $this->table = 'svf_multi';              // name of database table corresponding to instrument
        // data keyed by commentID
        $this->commentID = $commentID;
        
        //The array of dates/timestamps to convert to database dates/timestamps
        //Any HTML_Quickform date elements must be listed here
        $this->dateTimeFields=array("Date_taken", "clinical_1_date", "moca_date", "diagnosis_date", "onset_date");
            
        //The array of selects with multiple answers allowed
        //Any HTML_Quickform multiple selects must be listed here
        $this->_selectMultipleElements = array("region_of_onset");

        // required fields for data entry completion status
        $this->_requiredElements = array('Examiner', 'Date_taken','moca');

        // setup the form
        $this->_setupForm();

    }
    
    //If the instrument is not paged, remove the switch from the _setupForm method and add all the form Elements in this function.
    
    /**
    * method to build the HTML_Quickform object into a paged form
    *
    * @return void
    * @access private
    */
    function _setupForm(){
       if(preg_match("/svf_multi(_page[0-9]+)/",$this->page,$matches)){
            call_user_func(array($this, $matches[1]));
        } else {
            $this->_main();

        }
        //Defines the call back function for HTML Quickform to use when validating the form.

        $this->form->addFormRule(array(&$this,'XINValidate'));
    }
    
    /**
    * generates the main page of the form.
    *
    * @return void
    * @access private
    *
    */
    function _main(){
        // display test name
        $this->addHeader("Screen Visit Form");
        
        // automatically adds examiner & date of administration
        $this->_addMetadataFields();
        $yes_no_option= array(NULL=>"", "yes"=>"Yes", "no"=>"No", "not_answered"=>"Not Answered");

        // Top part of the form
    	$gender_options= array(NULL=>"", "male"=>"Male","female"=>"Female", "not_answered"=>"Not Answered");
    	$hand_preference_options= array(NULL=>"", "right"=>"Right","left"=>"Left", "ambidextrous"=>"Ambidextrous","not_answered"=>"Not Answered");
        $preferred_language_options= array(NULL=>"", "english"=>"English","french"=>"French", "other"=>"Other","not_answered"=>"Not Answered");
        $education_options = array(NULL=>"", "elementary"=>"Elementary", "high_school"=>"High School","post_secondary"=>"Post-Secondary","declined"=>"Declined","not_answered"=>"Not Answered");
        $communication_options = array(NULL=>"","gestures"=>"Gestures","writing"=>"Writing","technology"=>"Technology","other"=>"Other");
        $clinical_options = array(NULL=>"","yes"=>"Yes","past"=>"Past","unknown"=>"Unknown","never"=>"Never");

         // Bottom part of the form
        $onset_options = array(NULL=>"","bulbar"=>"Bulbar","speech"=>"Speech","swallowing"=>"Swallowing","upper_extremity"=>"Upper Extremity","lower_extremity"=>"Lower Extremity","repiratory"=>"Respiratory","ftd_cognitive"=>"FTD/Cognitive","not_available"=>"N/A");
        $first_motor_onset_options = array(NULL=>"","right"=>"Right","left"=>"Left","both"=>"Both","unknown"=>"Unknown","not_available","N/A");
        $first_symptom_options = array(NULL=>"","weakness"=>"Weakness","fasciculations"=>"Fasciculations","atrophy"=>"Atrophy","cramping"=>"Cramping","stiffness"=>"Stiffness","other"=>"Other");


        // Gender
        $this->form->addElement('select', 'gender', 'Gender:', $gender_options);

        // Hand Preference
        $this->form->addElement('select', 'hand_preference', 'Hand Preference:', $hand_preference_options);

        // Preferred Language
        $this->form->addElement('select', 'preferred_language', 'Preferred Lanuage:', $preferred_language_options);

        // Highest Level of education
        $this->form->addElement('select', 'highest_education', 'Highest level of education:', $education_options);

        // Years of education beginning in Grade 1
        $this->form->addElement('numeric','e_years', 'Years of education beginning in Grade 1: ');
        $this->form->addRule('e_years','Numerical value required.', 'numeric');

        // Age participant began learning English
        $this->form->addElement('numeric','a_english', 'Age participant began learning English: ');
        $this->form->addRule('a_english','Numerical value required.', 'numeric');

        // Is participant able to read English?
        $this->form->addElement('select', 'read_english', 'Is participant able to read English:', $yes_no_option);

        // Birthplace (city,country)
        $this->form->addElement('text','birth_place', 'Birthplace (City,Country):');

        // raised (city,country)
        $this->form->addElement('text','raised_place', 'Where was participant primarily raised (City,Country)?');

        // Does participant communicate by speaking?
        $this->form->addElement('select', 'communication', 'Does participant communicate by speaking?', $yes_no_option);
        $this->form->addElement('select', 'communication_type', 'If no, what is used to communicate?', $communication_options);
        $this->form->addElement('textarea','c_list', 'If other, list:');
        $this->XINRegisterRule("communication_type", array("communication{@}=={@}no"), "A type of communication must be specified.");
        $this->XINRegisterRule("c_list", array("communication_type{@}=={@}other"), "Other communication(s) must be specified.");

        // Experienced changes
        $this->form->addElement('select', 'e_changes', 'Has participant experienced changes in his/her speech in the past 2 years?', $yes_no_option);

        // Clinical Trials
        $this->form->addElement('select', 'clinical_trials', 'Is participant currently in a clinical trial?', $clinical_options);
        $this->form->addElement('text','clinical_1', 'If yes, name of clinical trial:');
        $this->form->addElement('date', 'clinical_1_date', 'Date of clinical trial mentioned above:');
        $this->XINRegisterRule("clinical_1", array("clinical_trials{@}=={@}yes"), "A clinical trial name is required.");
        $this->XINRegisterRule("clinical_1_date", array("clinical_trials{@}=={@}yes"), "A clinical trial date is required.");

        // Is participant a patient
        
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('select', 'is_patient', 'Is participant a patient?', $yes_no_option);
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');

        // Onset information
        $this->form->addElement('static', null, 'Onset information (patients only)');

        // Symptom onset date
        $this->form->addElement('date', 'onset_date', 'Symptom onset date:');
        $this->XINRegisterRule("onset_date", array("is_patient{@}=={@}yes"), "A symptom onset date must be provided.");

        // Diagnosis date
        $this->form->addElement('date', 'diagnosis_date', 'Diagnosis date:');
        $this->XINRegisterRule("diagnosis_date", array("is_patient{@}=={@}yes"), "A diagnosis date must be provided.");

        // Region of onset
        $this->form->addElement('select', 'region_of_onset', 'Region of onset (check all that apply):', $onset_options, array('multiple' => 'multiple'));
        $this->XINRegisterRule("region_of_onset", array("is_patient{@}=={@}yes"), "A region of onset(s) must be provided.");

        // Side of first motor symptom onset
        $this->form->addElement('select', 'first_motor_onset', 'Side of first motor symptom onset:', $first_motor_onset_options);
        $this->XINRegisterRule("first_motor_onset", array("is_patient{@}=={@}yes"), "A first motor onset(s) must be provided.");

        // First symptom (if limb onset)
        $this->form->addElement('select', 'first_symptom', 'First symptom (if limb onset):', $first_symptom_options);
        $this->form->addElement('textarea','s_list', 'If other, list:');
        $this->XINRegisterRule("first_symptom", array("is_patient{@}=={@}yes"), "A first symptom(s) must be provided.");
        $this->XINRegisterRule("s_list", array("first_symptom{@}=={@}other"), "Please list the first symptom(s).");

        // Notes
        $this->form->addElement('textarea','notes', 'Provide any additional notes:');
        $this->XINRegisterRule("notes",array("notes{@}=={@}NEVER_REQUIRED"),"Not required");

    }
   
    function _page1(){
        
       //add form Elements here as needed.
       //continue onto further pages, if needed.
     	$this->addHeader("Neuropsychometric Evaluations");
        // Is MoCA done?
        $this->form->addElement('select', 'moca', 'Montreal Cognitive Assessment (MoCA):', array(NULL=>"","done"=>"Done","not_done"=>"Not Done")); 

        // Date
        $this->form->addElement('date', 'moca_date', 'Date:');
        $this->XINRegisterRule("moca_date", array("moca{@}=={@}done"), "A diagnosis date must be provided.");

        // Type
        $this->form->addElement('select', 'type', 'Type:', array(NULL=>"","spoken"=>"Spoken","written"=>"Written"));
        $this->XINRegisterRule("type", array("moca{@}=={@}done"), "A type must be provided.");

        // Visuospatial/Executive Score
        $this->form->addElement('numeric','visuospatial', 'Visuospatial/Executive Score:');
        $this->form->addElement('select','modification', $this->indent . 'Modifications drawings used?', $yes_no_option);
        $this->XINRegisterRule("modification", array("moca{@}=={@}done"), "An answer must be provided.");
        $this->form->addRule('visuospatial','Numerical value required.', 'numeric');
        $this->XINRegisterRule("visuospatial", array("moca{@}=={@}done"), "A score must be provided.");

        // Naming score
        $this->form->addElement('numeric','naming_score', 'Naming Score:');
        $this->form->addRule('naming_score','Numerical value required.', 'numeric');
        $this->XINRegisterRule("naming_score", array("moca{@}=={@}done"), "A score must be provided.");

        // Attention score
        $this->form->addElement('numeric','attention_score', 'Attention Score:');
        $this->form->addRule('attention_score','Numerical value required.', 'numeric');
        $this->XINRegisterRule("attention_score", array("moca{@}=={@}done"), "A score must be provided.");

        // Language score
        $this->form->addElement('numeric','language_score', 'Language Score:');
        $this->form->addRule('language_score','Numerical value required.', 'numeric');
        $this->XINRegisterRule("language_score", array("moca{@}=={@}done"), "A score must be provided.");

        // Abstraction score
        $this->form->addElement('numeric','abstraction_score', 'Abstraction Score:');
        $this->form->addRule('abstraction_score','Numerical value required.', 'numeric');
        $this->XINRegisterRule("abstraction_score", array("moca{@}=={@}done"), "A score must be provided.");

        // Delayed recall score
        $this->form->addElement('numeric','delayed_recall_score', 'Delayed Recall Score:');
        $this->form->addRule('delayed_recall_score','Numerical value required.', 'numeric');
        $this->XINRegisterRule("delayed_recall_score", array("moca{@}=={@}done"), "A score must be provided.");

        // Orientation score
        $this->form->addElement('numeric','orientation_score', 'Orientation Score:');
        $this->form->addRule('orientation_score','Numerical value required.', 'numeric');
        $this->XINRegisterRule("orientation_score", array("moca{@}=={@}done"), "A score must be provided.");

        // Total Score
        $this->form->addElement('numeric','total_score', 'Total Score (+1 point if years of education is <12):');
        $this->form->addRule('total_score','Numerical value required.', 'numeric');
        $this->XINRegisterRule("total_score", array("moca{@}=={@}done"), "A score must be provided.");
    }
   
}
?>
