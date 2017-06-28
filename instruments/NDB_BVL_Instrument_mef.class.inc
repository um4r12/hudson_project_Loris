<?php
class NDB_BVL_Instrument_mef extends NDB_BVL_Instrument
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
        $this->testName = "mef";           // test_names.Test_name
        $this->table = 'mef';             // name of table containing


        // data keyed by commentID
        $this->commentID = $commentID;

        $this->_requiredElements = array('Date_taken', 'Examiner', 'form_type_current_medications', 'form_type_history');
        
        $this->dateTimeFields=array("Date_taken", "riluzole_date", 'clinical_1_date');
        
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
        $this->form->addElement('header', 'instrument_title', "M<span style='font-size: 26px'>EDICAL </span>E<span style='font-size: 26px'>VALUATIONS</span> F<span style='font-size: 26px'>ORM</span>");
        $this->form->addElement('static', null, $hr_header);
      
        // automatically adds examiner & date of administration
        $this->_addMetadataFields();
        $this->form->addElement('static', null, $hr_header);

        $yes_no_option= array(NULL=>"", "yes"=>"Yes", "no"=>"No", "not_answered"=>"Not Answered");
    	$normal_option= array(NULL=>"", "initial"=>"Initial","update"=>"Update", "no_change"=>"No Change", "not_answered"=>"Not Answered");

    	


    // Current Medications

        
        $this->form->addElement('select', 'form_type_current_medications', $this->indent."C<span style='font-size: 12px'>URRENT </span>M<span style='font-size: 12px'>EDICATIONS</span>", $normal_option);

        $this->form->addElement('static', null, $hr_section);

        // If form type is initial, all non-dependent fields are required with the exception of other
        // medications and recreational drug use.

            // Rule: Entry of Riluzole taken is required if the form type is initial.

                 $this->XINRegisterRule("riluzole", array("form_type_current_medications{@}=={@}initial"), "Please indicate whether or not Riluzole was taken.");


            // Rule: Entry of Anticonvulsants taken is required if the form type is initial.

                $this->XINRegisterRule("anticonvulsants", array("form_type_current_medications{@}=={@}initial"), "Please indicate whether or not Anticonvulsants were taken.");

            // Rule: Entry of Psychiatric Medications taken is required if the form type is initial.

                $this->XINRegisterRule("psych_medications", array("form_type_current_medications{@}=={@}initial"), "Please indicate whether or not Psychiatric Medications were taken.");

            // Rule: Entry of Benzodiazepenes taken is required if the form type is initial.

                $this->XINRegisterRule("benzodiazepines", array("form_type_current_medications{@}=={@}initial"), "Please indicate whether or not Benzodiazepines were taken.");

            // Rule: Entry of Opiates taken is required if the form type is initial.

                 $this->XINRegisterRule("opiates", array("form_type_current_medications{@}=={@}initial"), "Please indicate whether or not Opiates were taken.");

            // Rule: Entry of Baclofen taken is required if the form type is initial.

                $this->XINRegisterRule("baclofen", array("form_type_current_medications{@}=={@}initial"), "Please indicate whether or not Baclofen was taken.");

            // Rule: Entry of Current Alcohol Consumption is required if the form type is initial.

                $this->XINRegisterRule("alcohol", array("form_type_current_medications{@}=={@}initial"), "Please provide current alcohol consumption. If no current alcohol <br>consumption, enter 0.");

            // Rule: Entry of Other medications is never required. It will only be entered if there are any medications, otherwise will be left blank.

                $this->XINRegisterRule("other_medication",array("other_medication{@}=={@}NEVER_REQUIRED"),"Not required");

            // Rule: Entry of Recreational Drug Use is never required. It will only be entered if there is any recreational drug use, otherwise will be left blank.

                $this->XINRegisterRule("recreational_drugs",array("recreational_drugs{@}=={@}NEVER_REQUIRED"),"Not required");

            // Rule: Entry of participation in clinical trial is required if the form type is initial.

                $this->XINRegisterRule("clinical_trials", array("form_type_current_medications{@}=={@}initial"), "Please indicate whether or not candidate is participating in a clinical trial.");
     
            // Rule: Entry of Current Medication Notes is never required. Suggested by Dennell. Previously was set to required when set to update.

                $this->XINRegisterRule("current_medications_notes", array("current_medications_notes{@}=={@}NEVER_REQUIRED"), "Not required.");


        // Riluzole

            // Entry for Riluzole taken

                $this->form->addElement('select', 'riluzole', $this->indent . 'Riluzole:', $yes_no_option);

            // Entry for Riluzole start date

       	        $this->form->addElement('date', 'riluzole_date', $this->indent . $this->indent ."<span style='font-weight:normal'><i>If yes, Riluzole start date:</i></span>");

            // Rule: Entry of Riluzole start date is required if Riluzole taken is set to yes. 

                $this->XINRegisterRule("riluzole_date", array("riluzole{@}=={@}yes"), "A Riluzole start date is required.");


        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Anticonvulsants

            // Entry for Anticonvulsants taken

                $this->form->addElement('select', 'anticonvulsants', $this->indent . 'Anticonvulsants:', $yes_no_option);

            // Entry for type(s) and dose(s) of Anticonvulsants

                $this->form->addElement('textarea','ac_medication', $this->indent . $this->indent . "<span style='font-weight:normal'><i>If yes, list medication and dose:</i></span>");

            // Rule: Entry for type(s) and dose(s) of Anticonvulsants required if Anticonvulsants taken is set to yes.

                $this->XINRegisterRule("ac_medication",array("anticonvulsants{@}=={@}yes"),"Medication(s) and dose(s) are required.");

        
        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);
            

        // Psychiatric medications

            // Entry for Psychiatric Medications taken

                $this->form->addElement('select', 'psych_medications', $this->indent . 'Psychiatric Medications:', $yes_no_option);

            // Entry for type(s) and dose(s) of Psychiatric Medications

                $this->form->addElement('textarea','p_medication', $this->indent . $this->indent . "<span style='font-weight:normal'><i>If yes, list medication and dose:</i></span>");

            // Rule: Entry for type(s) and dose(s) of Psychiatric Medications required if Psychiatric Medications taken is set to yes.
                
                $this->XINRegisterRule("p_medication", array("psych_medications{@}=={@}yes"), "Medication(s) and dose(s) are required.");


        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Benzodiazepines

            // Entry for Benzodiazepines taken

                $this->form->addElement('select', 'benzodiazepines', $this->indent . 'Benzodiazepines:', $yes_no_option);

            // Entry for type(s) and dose(s) of Benzodiazepines

                $this->form->addElement('textarea','b_medication', $this->indent . $this->indent . "<span style='font-weight:normal'><i>If yes, list medication and dose:</i></span>");

            // Rule: Entry for type(s) and dose(s) of Benzodiazepines required if Benzodiazepines taken is set to yes.

                $this->XINRegisterRule("b_medication", array("benzodiazepines{@}=={@}yes"), "Medication(s) and dose(s) are required.");

        
        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Opiates

            // Entry for Opiates taken

                $this->form->addElement('select', 'opiates', $this->indent . 'Opiates:', $yes_no_option);

            // Entry for type(s) and dose(s) of Opiates

                $this->form->addElement('textarea','o_medication', $this->indent . $this->indent . "<span style='font-weight:normal'><i>If yes, list medication and dose:</i></span>");

            // Rule: Entry for type(s) and dose(s) of Opiates required if Opiates taken is set to yes.

                $this->XINRegisterRule("o_medication", array("opiates{@}=={@}yes"), "Medication(s) and dose(s) are required.");
      

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Baclofen

            // Entry for Baclofen taken

                $this->form->addElement('select', 'baclofen', $this->indent . 'Baclofen:', $yes_no_option);

            // Entry for type(s) and dose(s) of Baclofen

                $this->form->addElement('textarea','bac_medication', $this->indent . $this->indent . "<span style='font-weight:normal'><i>If yes, list medication and dose:</i></span>");

            // Rule: Entry for type(s) and dose(s) of Baclofen required if Baclofen taken is set to yes.

                $this->XINRegisterRule("bac_medication", array("baclofen{@}=={@}yes"), "Medication(s) and dose(s) are required.");


        // Horizontal Line Break      

            $this->form->addElement('static', null, $hr_section);


        // Other medication(s) and dose(s) if known

            // Entry for other medication(s) taken

                $this->form->addElement('textarea','other_medication', $this->indent . 'List other medication(s) and dose(s) if known:');

        
        // Horizontal Line Break      

            $this->form->addElement('static', null, $hr_section);


        // Current alcohol consumption

            // Entry for Current Alcohol Consumption

                $this->form->addElement('numeric','alcohol', $this->indent . 'Current alcohol consumption (avg. # of ounces/week):');

            // Rule: Current Alcohol Consumption must be a numerical value.

                $this->form->addRule('alcohol','Enter a numerical value. If no current alcohol consumption, enter 0.', 'numeric');


        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

            
        // Recreational Drugs

            // Entry for Recreational Drug Use
                $this->form->addElement('textarea','recreational_drugs', $this->indent . 'List any recreational drug use (type & quantity per week):');


        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);
        
        
        // Clinical Trials

            // Entry for Clinical Trials

                $this->form->addElement('select', 'clinical_trials', $this->indent . 'Is participant currently in a clinical trial?', $yes_no_option);

            // Entry for name of Clinical Trial 

                $this->form->addElement('text','clinical_1', $this->indent . $this->indent . "<span style='font-weight:normal'><i>If yes, name of clinical trial:</i></span>");

            // Entry for start date of Clinical Trial

                $this->form->addElement('date', 'clinical_1_date', $this->indent . $this->indent . "<span style='font-weight:normal'><i>Start date of clinical trial mentioned above:</i></span>");

            // Rule: Entry for name of Clinical Trial is required if Clinical Trials is set to yes

                $this->XINRegisterRule("clinical_1", array("clinical_trials{@}=={@}yes"), "A clinical trial name is required.");

            // Rule: Entry for date of Clinical Trial is required if Clinical Trials is set to yes

                $this->XINRegisterRule("clinical_1_date", array("clinical_trials{@}=={@}yes"), "A clinical trial date is required.");


        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Current Medication Notes

            // Entry for any notes

                $this->form->addElement('textarea','current_medications_notes', $this->indent.'Notes:');

    
        // Horizontal Line Section Break

            $this->form->addElement('static', null, "<br>".$hr_header."<br>");


        

       
    // Medical Conditions/History
        
        $this->form->addElement('select', 'form_type_history', $this->indent."M<span style='font-size: 12px'>EDICAL </span>C<span style='font-size: 12px'>ONDITIONS </span>H<span style='font-size: 12px'>ISTORY</span>", $normal_option);
        $this->form->addElement('static', null, $hr_section);

        // If form type is initial, all non-dependent fields are required.

            // Rule: Entry of Stroke is required if the form type is initial.

                $this->XINRegisterRule("stroke", array("form_type_history{@}=={@}initial"), "This field is required.");

            // Rule: Entry of Brain Truama is required if the form type is initial.

                $this->XINRegisterRule("brain_trauma", array("form_type_history{@}=={@}initial"), "This field is required.");

            // Rule: Entry of Cognitive & Learning issues is required if the form type is initial.

                $this->XINRegisterRule("c_l_issues", array("form_type_history{@}=={@}initial"), "This field is required.");

            // Rule: Entry of Epilepsy is required if the form type is initial.

                $this->XINRegisterRule("epilepsy", array("form_type_history{@}=={@}initial"), "This field is required.");

            // Rule: Entry of Other Neurological Conditions is required if the form type is initial.

                $this->XINRegisterRule("o_neuro_conditions", array("form_type_history{@}=={@}initial"), "This field is required.");

            // Rule: Entry of Depression is required if the form type is initial.

                $this->XINRegisterRule("depression", array("form_type_history{@}=={@}initial"), "This field is required.");

            // Rule: Entry of Other Psychiatric Conditions is required if the form type is initial.

                $this->XINRegisterRule("o_p_conditions", array("form_type_history{@}=={@}initial"), "This field is required.");

            // Rule: Entry of Malignancy is required if the form type is initial. 

                $this->XINRegisterRule("malignancy", array("form_type_history{@}=={@}initial"), "This field is required.");

            // Rule: Entry of Diabetes is required if the form type is initial.

                $this->XINRegisterRule("diabetes", array("form_type_history{@}=={@}initial"), "This field is required.");

            // Rule: Entry of Hypertension is required if the form type is initial.

                $this->XINRegisterRule("hypertension", array("form_type_history{@}=={@}initial"), "This field is required.");

            // Rule: Entry of Other Significant Medication Conditions is required if the form type is initial.

                $this->XINRegisterRule("o_s_conditions", array("form_type_history{@}=={@}initial"), "This field is required.");

            // Rule: Entry of Medical Conditions History Notes is required if the form type is set to update.

                $this->XINRegisterRule("medical_conditions_history_notes", array("medical_conditions_history_notes{@}=={@}NEVER_REQUIRED"), "Never required.");
  


        // Stroke

            // Entry for Stroke

                $this->form->addElement('select', 'stroke', $this->indent.'Stroke:' ,$yes_no_option);


        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);
        

        // Brain trauma

            // Entry for Brain Trauma

                $this->form->addElement('select', 'brain_trauma', $this->indent.'Brain Trauma:', $yes_no_option);


        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);
        

        // Cognitive_learning issues

            // Entry for Cognitive/Learning issues

                $this->form->addElement('select', 'c_l_issues', $this->indent.'Cognitive/learning issues:', $yes_no_option);


        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Epilepsy

            // Entry for Epilepsy

                $this->form->addElement('select', 'epilepsy', $this->indent.'Epilepsy:', $yes_no_option);


        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Other neuro conditions

            // Entry for Other Neurological Conditions

                $this->form->addElement('select', 'o_neuro_conditions', $this->indent.'Other neurological conditions:', $yes_no_option);

            // Entry for name of Other Neurological Conditions

                $this->form->addElement('text','n_conditions', $this->indent.$this->indent."<span style='font-weight:normal'><i>If yes, specify condition:</i></span>");

            // Rule: Entry for name of Other Neurological Conditions is required if Entry for Other Neurological Conditions is set to yes

                $this->XINRegisterRule("n_conditions", array("o_neuro_conditions{@}=={@}yes"), "A condition must be specified.");


        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Depression

            // Entry for Depression

                $this->form->addElement('select', 'depression', $this->indent.'Depression:', $yes_no_option);


        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Other psychiatric conditions

            // Entry for Other Psychiatric Conditions

                $this->form->addElement('select', 'o_p_conditions', $this->indent.'Other psychiatric conditions:', $yes_no_option);

            // Entry for name of Other Psychiatric Conditon

                $this->form->addElement('text','p_conditions', $this->indent.$this->indent."<span style='font-weight:normal'><i>If yes, specify condition:</i></span>");

            // Rule: Entry for name Other Psychiatric Condition is required if Other Psychiatric Condition is set to yes

                $this->XINRegisterRule("p_conditions", array("o_p_conditions{@}=={@}yes"), "A condition must be specified.");


         // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Malignancy

            // Entry for Malignancy

                $this->form->addElement('select', 'malignancy', $this->indent.'Malignancy:', $yes_no_option);

            // Entry for type of Malignancy

                $this->form->addElement('text','m_type', $this->indent.$this->indent."<span style='font-weight:normal'><i>If yes, specify type:</i></span>");

            // Entry for year of Malignancy

                $this->form->addElement('numeric','m_year', $this->indent.$this->indent."<span style='font-weight:normal'><i>If yes, specify year:</i></span>");

                // Rule: Makes sure it is a numerical field.

                    $this->form->addRule('m_year','Numerical value required.', 'numeric');

            // Entry for Chemo/radiation used

                $this->form->addElement('select', 'chemo', $this->indent.$this->indent."<span style='font-weight:normal'><i>If yes, was chemo/radiation used?</i></span>", $yes_no_option);

            // Rule: Rule for type of Malignancy to be required if Malignancy is set to yes.                        

                $this->XINRegisterRule("m_type", array("malignancy{@}=={@}yes"), "A type must be specified.");

            // Rule: Rule for year of Malignancy to be required if Malignancy is set to yes.      

                $this->XINRegisterRule("m_year", array("malignancy{@}=={@}yes"), "A year must be provided.");

            // Rule: Rule for Chemo/radiation used to be required if Malignancy is set to yes.  

                $this->XINRegisterRule("chemo", array("malignancy{@}=={@}yes"), "An answer must be specified.");



        // Horizontal Line Break
                    
            $this->form->addElement('static', null, $hr_section);



        // Diabetes

            // Entry for Diabetes

                $this->form->addElement('select', 'diabetes', $this->indent.'Diabetes:', $yes_no_option);


        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Hypertension

            // Entry for Hypertension
        
                $this->form->addElement('select', 'hypertension', $this->indent.'Hypertension:', $yes_no_option);



        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);



        // Other significant medical conditions

            // Entry for Other Significant Medical Conditions

                $this->form->addElement('select', 'o_s_conditions', $this->indent.'Other significant medical conditions:', $yes_no_option);

            // Entry for specific Other Significant Medical Conditions

                $this->form->addElement('text','s_conditions', $this->indent.$this->indent."<span style='font-weight:normal'><i>If yes, specify condition:</i></span>");

            // Rule: Rule for specific Other Significant Medical Conditions to be required if Other Significant Medical Conditions is set to yes

                $this->XINRegisterRule("s_conditions", array("o_s_conditions{@}=={@}yes"), "A condition must be specified.");


        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        
        
        // Notes
               
            // Entry for Medical Conditions History Notes 
        
                 $this->form->addElement('textarea','medical_conditions_history_notes', $this->indent.'Notes:');


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
