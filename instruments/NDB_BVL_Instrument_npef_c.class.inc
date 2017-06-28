<?php
class NDB_BVL_Instrument_npef_c extends NDB_BVL_Instrument
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
        $this->testName = "npef_c";           // test_names.Test_name
        $this->table = 'npef_c';             // name of table containing


        // data keyed by commentID
        $this->commentID = $commentID;

        $this->_requiredElements = array('Date_taken', 'Examiner','ecas_done','semantic_done','digit_done','bnt_done','hvltr_done','social_questionnaire_done','hads_done','cns_done','victoria_done','jlo_done','social_behavior_done');
        
        $this->dateTimeFields=array("Date_taken", "ecas_date", "semantic_date","digit_date","bnt_date","hvltr_date","social_questionnaire_date","hads_date","cns_date","victoria_date","jlo_date","social_behavior_date");
        //$this->_selectMultipleElements = array("<variable>");
        
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

        //Defines the call back function for HTML Quickform to use when validating the form.
        $this->form->addFormRule(array(&$this,'XINValidate'));

        // display test name
        $this->form->addElement('header', 'npef_c', "Neuropsychometric Evaluations Form");
       $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');


        // automatically adds examiner & date of administration
        $this->_addMetadataFields();

        $yes_no_option= array(NULL=>"", "yes"=>"Yes", "no"=>"No", "not_answered"=>"Not Answered");
        $ecas_version_option = array(NULL=>"","v_one"=>"1","v_two"=>"2","v_three"=>"3");
        $hvltr_version_option = array(NULL=>"","v_one"=>"1","v_two"=>"2");
    	$general_spoken_written_option= array(NULL=>"", "spoken"=>"Spoken","written"=>"Written");
        $victoria_test_option= array(NULL=>"", "spoken"=>"Spoken","color"=>"Colour Box Pointing");
        $done_option=array(NULL=>"", "done"=>"Done","not_done"=>"Not Done");
        $semnatic_fluency_option = array(NULL=>"","animal"=>"Animal","fruit"=>"Fruit","vegetable"=>"Vegetable");
        $jlo_version_option = array(NULL=>"","v_one"=>"V","v_two"=>"H");
        $bnt_version_option=array(NULL=>"","short"=>"Short form","long"=>"Standard (long) form");

        // Breaks for formatting
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
    	
        // ECAS         
        $this->form->addElement('static', null, '<font size=4>Edinburgh Cognitive and Behavioral ALS Screen (ECAS)</font>');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('select', 'ecas_done',$this->indent . $this->indent . 'Has the ECAS been done?', $done_option);

        // ECAS Done/Not Done Rules
        $this->XINRegisterRule("ecas_date", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("ecas_type", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("ecas_version", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("language_naming", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("language_comprehension", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("language_spelling", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("verbal_s", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("verbal_t", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("executive_reverse", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("executive_alternation", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("executive_social", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("executive_sentence", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("memory_immediate", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("memory_delayed_retention", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("memory_delayed_recognition", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("visuospatial_dot", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("visuospatial_cube", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("visuospatial_number", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("ecas_total_language", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("ecas_total_verbal", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("speech_date", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("ecas_total_executive", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("ecas_total_memory", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("ecas_total_visuospatial", array("ecas_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("ecas_total_final", array("ecas_done{@}=={@}done"), "Required.");




        // ECAS Questions
        $this->form->addElement('date', 'ecas_date', $this->indent . $this->indent . 'Date of ECAS:');
        $this->form->addElement('select', 'ecas_type',$this->indent .  $this->indent . 'Type of test:', $general_spoken_written_option);

        $this->form->addElement('select', 'ecas_version',$this->indent .  $this->indent . 'Test Version:', $ecas_version_option);

        $this->form->addElement('static', null, $this->indent . 'Language');

            $this->form->addElement('text', 'language_naming', $this->indent . $this->indent . $this->indent . $this->indent . 'Naming:');
            $this->form->addRule('language_naming','Numerical value required.', 'numeric');

            $this->form->addElement('text', 'language_comprehension', $this->indent . $this->indent . $this->indent . $this->indent . 'Comprehension:');
            $this->form->addRule('language_comprehension','Numerical value required.', 'numeric');

            $this->form->addElement('text', 'language_spelling', $this->indent . $this->indent . $this->indent . $this->indent . 'Spelling:');
            $this->form->addRule('language_spelling','Numerical value required.', 'numeric');

        $this->form->addElement('static', null, $this->indent . 'Verbal Fluency');

            $this->form->addElement('text', 'verbal_s', $this->indent . $this->indent . $this->indent . $this->indent . 'First Letter (S/F/P):');
            $this->form->addRule('verbal_s','Numerical value required.', 'numeric');

            $this->form->addElement('text', 'verbal_t', $this->indent . $this->indent . $this->indent . $this->indent . 'Second Letter(T/D/M):');
            $this->form->addRule('verbal_t','Numerical value required.', 'numeric');

         $this->form->addElement('static', null, $this->indent . 'Executive');

            $this->form->addElement('text', 'executive_reverse', $this->indent . $this->indent . $this->indent . $this->indent . 'Reverse digit span:');
            $this->form->addRule('executive_reverse','Numerical value required.', 'numeric');

            $this->form->addElement('text', 'executive_alternation', $this->indent . $this->indent . $this->indent . $this->indent . 'Alternation:');
            $this->form->addRule('executive_alternation','Numerical value required.', 'numeric');

            $this->form->addElement('text', 'executive_social', $this->indent . $this->indent . $this->indent . $this->indent . 'Social Cognitiion:');
            $this->form->addRule('executive_social','Numerical value required.', 'numeric');

            $this->form->addElement('text', 'executive_sentence', $this->indent . $this->indent . $this->indent . $this->indent . 'Sentence Completion:');
            $this->form->addRule('executive_sentence','Numerical value required.', 'numeric');

        $this->form->addElement('static', null, $this->indent . 'Memory');

            $this->form->addElement('text', 'memory_immediate', $this->indent . $this->indent . $this->indent . $this->indent . 'Immediate recall:');
            $this->form->addRule('memory_immediate','Numerical value required.', 'numeric');

            $this->form->addElement('text', 'memory_delayed_retention', $this->indent . $this->indent . $this->indent . $this->indent . 'Delayed retention:');
            $this->form->addRule('memory_delayed_retention','Numerical value required.', 'numeric');

            $this->form->addElement('text', 'memory_delayed_recognition', $this->indent . $this->indent . $this->indent . $this->indent . 'Delayed recognition:');
            $this->form->addRule('memory_delayed_recognition','Numerical value required.', 'numeric');

        $this->form->addElement('static', null, $this->indent . 'Visuospatial');

            $this->form->addElement('text', 'visuospatial_dot', $this->indent . $this->indent . $this->indent . $this->indent . 'Dot Counting:');
            $this->form->addRule('visuospatial_dot','Numerical value required.', 'numeric');

            $this->form->addElement('text', 'visuospatial_cube', $this->indent . $this->indent . $this->indent . $this->indent . 'Cube Counting:');
            $this->form->addRule('visuospatial_cube','Numerical value required.', 'numeric');

            $this->form->addElement('text', 'visuospatial_number', $this->indent . $this->indent . $this->indent . $this->indent . 'Number Location:');
            $this->form->addRule('visuospatial_number','Numerical value required.', 'numeric');

        $this->form->addElement('static', null, $this->indent . 'Domain Totals');

            $this->form->addElement('text', 'ecas_total_language', $this->indent . $this->indent . $this->indent . $this->indent . 'Language:');
            $this->form->addRule('ecas_total_language','Numerical value required.', 'numeric');

            $this->form->addElement('text', 'ecas_total_verbal', $this->indent . $this->indent . $this->indent . $this->indent . 'Verbal Fluency:');
            $this->form->addRule('ecas_total_verbal','Numerical value required.', 'numeric');

            $this->form->addElement('text', 'ecas_total_executive', $this->indent . $this->indent . $this->indent . $this->indent . 'Executive:');
            $this->form->addRule('ecas_total_executive','Numerical value required.', 'numeric');

            $this->form->addElement('text', 'ecas_total_memory', $this->indent . $this->indent . $this->indent . $this->indent . 'Memory:');
            $this->form->addRule('ecas_total_memory','Numerical value required.', 'numeric');

            $this->form->addElement('text', 'ecas_total_visuospatial', $this->indent . $this->indent . $this->indent . $this->indent . 'Visuospatial:');
            $this->form->addRule('ecas_total_visuospatial','Numerical value required.', 'numeric');

        $this->form->addElement('text', 'ecas_total_final', $this->indent . $this->indent . 'ECAS Total score');
        $this->form->addRule('ecas_total_final','Numerical value required.', 'numeric');



        // Breaks for formatting
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        
        // Semantic Fluency + Abrahams Correction         
        $this->form->addElement('static', null, '<font size=4>Semantic Fluency + Abrahams Correction</font>');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('select', 'semantic_done',$this->indent . $this->indent . 'Has the Semantic Fluency + Abrahams Correction been done?', $done_option);

        // Semantic Fluency + Abrahams Correction Done/Not Done Rules
        $this->XINRegisterRule("semantic_date", array("semantic_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("semantic_type", array("semantic_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("semantic_fluency", array("semantic_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("semantic_number", array("semantic_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("semantic_time", array("semantic_done{@}=={@}done"), "Required.");


        // Semantic Fluency + Abarahams Correction Questions
        $this->form->addElement('date', 'semantic_date', $this->indent . $this->indent . 'Date of test:');
        $this->form->addElement('select', 'semantic_type',$this->indent .  $this->indent . 'Type of test:', $general_spoken_written_option);
        $this->form->addElement('select', 'semantic_fluency',$this->indent .  $this->indent . 'Type of test:', $semnatic_fluency_option);

        $this->form->addElement('text', 'semantic_number', $this->indent . $this->indent . 'Number of correct words:');
        $this->form->addRule('semantic_number','Numerical value required.', 'numeric');

        $this->form->addElement('text', 'semantic_time', $this->indent . $this->indent . 'Time to read aloud/copy(s):');
        $this->form->addRule('semantic_time','Numerical value required.', 'numeric');



        // Breaks for formatting
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        
        // Digit Span
        $this->form->addElement('static', null, '<font size=4>Digit Span</font>');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('select', 'digit_done',$this->indent . $this->indent . 'Has the Digit Span test been done?', $done_option);

        // Digit Span Done/Not Done Rules
        $this->XINRegisterRule("digit_date", array("digit_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("digit_type", array("digit_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("digit_forward", array("digit_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("digit_dot_score", array("digit_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("digit_dot_maximal", array("digit_done{@}=={@}done"), "Required.");

        // Digit Span Questions
        $this->form->addElement('date', 'digit_date', $this->indent . $this->indent . 'Date of Digit Span test:');
        $this->form->addElement('select', 'digit_type',$this->indent .  $this->indent . 'Type of test:', $general_spoken_written_option);
        
        $this->form->addElement('text', 'digit_forward', $this->indent . $this->indent . 'Forward score:');
        $this->form->addRule('digit_forward','Numerical value required.', 'numeric');

        $this->form->addElement('text', 'digit_dot_score', $this->indent . $this->indent . 'DOT score:');
        $this->form->addRule('digit_dot_score','Numerical value required.', 'numeric');

        $this->form->addElement('text', 'digit_dot_maximal', $this->indent . $this->indent . 'DOT maximal span:');
        $this->form->addRule('digit_dot_maximal','Numerical value required.', 'numeric');


        // Breaks for formatting
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        
        // BNT
        $this->form->addElement('static', null, '<font size=4>Boston Naming Test - II (BNT)</font>');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('select', 'bnt_done',$this->indent . $this->indent . 'Has the Digit Span test been done?', $done_option);

        // BNT Done/Not Done Rules
        $this->XINRegisterRule("bnt_date", array("bnt_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("bnt_type", array("bnt_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("bnt_version", array("bnt_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("bnt_total", array("bnt_done{@}=={@}done"), "Required.");


        // BNT Questions
        $this->form->addElement('date', 'bnt_date', $this->indent . $this->indent . 'Date of BNT test:');
        $this->form->addElement('select', 'bnt_type',$this->indent .  $this->indent . 'Type of test:', $general_spoken_written_option);
        
        $this->form->addElement('select', 'bnt_version', $this->indent . $this->indent . 'Test Version:', $bnt_version_option);
        
        $this->form->addElement('text', 'bnt_total', $this->indent . $this->indent . 'BNT Total score:');
        $this->form->addRule('bnt_total','Numerical value required.', 'numeric');


        // Breaks for formatting
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        
        // HVLTR
        $this->form->addElement('static', null, '<font size=4>Hopkins Verbal Learning Test - Revised (HVLT-R)</font>');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('select', 'hvltr_done', $this->indent . $this->indent . 'Has the HVLT-R test been done?', $done_option);

        // HVLTR Done/Not Done Rules
        $this->XINRegisterRule("hvltr_date", array("hvltr_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("hvltr_type", array("hvltr_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("hvltr_version", array("hvltr_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("hvltr_total_recall", array("hvltr_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("hvltr_delayed", array("hvltr_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("hvltr_retention", array("hvltr_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("hvltr_recognition", array("hvltr_done{@}=={@}done"), "Required.");
        


        // HVLTR Questions
        $this->form->addElement('date', 'hvltr_date', $this->indent . $this->indent . 'Date of HVLT-R test:');
        $this->form->addElement('select', 'hvltr_type',$this->indent .  $this->indent . 'Type of test:', $general_spoken_written_option);
        
        $this->form->addElement('select', 'hvltr_version', $this->indent . $this->indent . 'Test Version:', $hvltr_version_option);
     

        $this->form->addElement('text', 'hvltr_total_recall', $this->indent . $this->indent . 'Total recall:');
        $this->form->addRule('hvltr_total_recall','Numerical value required.', 'numeric');

        $this->form->addElement('text', 'hvltr_delayed', $this->indent . $this->indent . 'Delayed recall:');
        $this->form->addRule('hvltr_delayed','Numerical value required.', 'numeric');

        $this->form->addElement('text', 'hvltr_retention', $this->indent . $this->indent . 'Retention (%):');
        $this->form->addRule('hvltr_retention','Numerical value required.', 'numeric');

        $this->form->addElement('text', 'hvltr_recognition', $this->indent . $this->indent . 'Recognition discrimination index:');
        $this->form->addRule('hvltr_recognition','Numerical value required.', 'numeric');

        // Breaks for formatting
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        
        // Social Questionnaire
        $this->form->addElement('static', null, '<font size=4>Social Norms Questionnaire Survey</font>');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('select', 'social_questionnaire_done',$this->indent . $this->indent . 'Has the questionnaire been done?', $done_option);

        // Social Questionnaire Done/Not Done Rules
        
        $this->XINRegisterRule("social_questionnaire_date", array("social_questionnaire_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("social_questionnaire_sum", array("social_questionnaire_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("social_questionnaire_snq22", array("social_questionnaire_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("social_questionnaire_break", array("social_questionnaire_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("social_questionnaire_overadhere", array("social_questionnaire_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("social_questionnaire_yn", array("social_questionnaire_done{@}=={@}done"), "Required.");


        // Social Questionnaire Questions
        $this->form->addElement('date', 'social_questionnaire_date', $this->indent . $this->indent . 'Date of Social Questionnaire:');
        

        $this->form->addElement('text', 'social_questionnaire_sum', $this->indent . $this->indent . 'Sum of items:');
        $this->form->addRule('social_questionnaire_sum','Numerical value required.', 'numeric');

        $this->form->addElement('text', 'social_questionnaire_snq22', $this->indent . $this->indent . 'SNQ22 Total Score:');
        $this->form->addRule('social_questionnaire_snq22','Numerical value required.', 'numeric');

        $this->form->addElement('text', 'social_questionnaire_break', $this->indent . $this->indent . 'Break Score:');
        $this->form->addRule('social_questionnaire_break','Numerical value required.', 'numeric');

        $this->form->addElement('text', 'social_questionnaire_overadhere', $this->indent . $this->indent . 'Overadhere Score:');
        $this->form->addRule('social_questionnaire_overadhere','Numerical value required.', 'numeric');

        $this->form->addElement('text', 'social_questionnaire_yn', $this->indent . $this->indent . 'Y/N Ratio Score:');
        $this->form->addRule('social_questionnaire_yn','Numerical value required.', 'numeric');

        // Breaks for formatting
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        
        // HADS 
        $this->form->addElement('static', null, '<font size=4>Hospital Anxiety and Depression Scale (HADS)</font>');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('select', 'hads_done',$this->indent . $this->indent . 'Has the HADS been done?', $done_option);

        // HADS Done/Not Done Rules
        
        $this->XINRegisterRule("hads_date", array("hads_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("hads_depression", array("hads_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("hads_anxiety", array("hads_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("hads_question_eight", array("hads_done{@}=={@}done"), "Required.");


        // HADS Questions
        $this->form->addElement('date', 'hads_date', $this->indent . $this->indent . 'Date of HADS:');
        

        $this->form->addElement('static', null, $this->indent . 'Total score:');

        $this->form->addElement('text', 'hads_depression', $this->indent . $this->indent . $this->indent . 'Depression (D):');
        $this->form->addRule('hads_depression','Numerical value required.', 'numeric');

        $this->form->addElement('text', 'hads_anxiety', $this->indent . $this->indent . $this->indent . 'Anxiety (A):');
        $this->form->addRule('hads_anxiety','Numerical value required.', 'numeric');

        $this->form->addElement('text', 'hads_question_eight', $this->indent . $this->indent . 'Score on Question #8 ("I feel as if I am slowed down"):');
        $this->form->addRule('hads_question_eight','Numerical value required.', 'numeric');

        // Breaks for formatting
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        
        // CNS-LS 
        $this->form->addElement('static', null, '<font size=4>Center for Neurological Study - Liability Scale (CNS-LS)</font>');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('select', 'cns_done',$this->indent . $this->indent . 'Has the CNS-LS been done?', $done_option);

        // CNS-LS Done/Not Done Rules
        $this->XINRegisterRule("cns_date", array("cns_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("cns_total", array("cns_done{@}=={@}done"), "Required.");



        // CNS-LS Questions
        $this->form->addElement('date', 'cns_date', $this->indent . $this->indent . 'Date of CNS-LS:');
        

        $this->form->addElement('text', 'cns_total', $this->indent . $this->indent . 'Total score:');
        $this->form->addRule('cns_total','Numerical value required.', 'numeric');


        // Breaks for formatting
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        
        // Vistoria Stroop Test 
        $this->form->addElement('static', null, '<font size=4>Victoria Stroop Test</font>');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('select', 'victoria_done',$this->indent . $this->indent . 'Has the Victoria Stroop Test been done?', $done_option);

        // Victoria Strrop Test Done/Not Done Rules
        $this->XINRegisterRule("victoria_date", array("victoria_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("victoria_type", array("victoria_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("victoria_one_time", array("victoria_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("victoria_one_errors", array("victoria_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("victoria_two_time", array("victoria_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("victoria_two_errors", array("victoria_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("victoria_three_time", array("victoria_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("victoria_three_errors", array("victoria_done{@}=={@}done"), "Required.");
        


        // Victoria Stroop Test Questions
        $this->form->addElement('date', 'victoria_date', $this->indent . $this->indent . 'Date of Victoria Stroop Test:');
        $this->form->addElement('select', 'victoria_type',$this->indent .  $this->indent . 'Type of test:', $victoria_test_option);
        

        $this->form->addElement('static', null, $this->indent . '<i><font size=3>Trial 1 (Color Dots)</font></i>');

            $this->form->addElement('text', 'victoria_one_time', $this->indent . $this->indent . $this->indent . 'Time taken(s): ');
            $this->form->addRule('victoria_one_time','Numerical value required.', 'numeric');

            $this->form->addElement('text', 'victoria_one_errors', $this->indent . $this->indent . $this->indent . '# of errors: ');
            $this->form->addRule('victoria_one_errors','Numerical value required.', 'numeric');

        $this->form->addElement('static', null, $this->indent . '<i><font size=3>Trial 2 (Color Words)</font></i>');

            $this->form->addElement('text', 'victoria_two_time', $this->indent . $this->indent . $this->indent . 'Time taken(s): ');
            $this->form->addRule('victoria_two_time','Numerical value required.', 'numeric');

            $this->form->addElement('text', 'victoria_two_errors', $this->indent . $this->indent . $this->indent . '# of errors: ');
            $this->form->addRule('victoria_two_errors','Numerical value required.', 'numeric');

        $this->form->addElement('static', null, $this->indent . '<i><font size=3>Trial 3 (Color-Words Interference)</font></i>');

            $this->form->addElement('text', 'victoria_three_time', $this->indent . $this->indent . $this->indent . 'Time taken(s): ');
            $this->form->addRule('victoria_three_time','Numerical value required.', 'numeric');

            $this->form->addElement('text', 'victoria_three_errors', $this->indent . $this->indent . $this->indent . '# of errors: ');
            $this->form->addRule('victoria_three_errors','Numerical value required.', 'numeric');

        // Breaks for formatting
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        
        // JLO
        $this->form->addElement('static', null, '<font size=4>Benton Judgement of Line Orientation (JLO)</font>');
        $this->form->addElement('static', null, $this -> indent. '<i>If ECAS Visuospatial Score <10 </i>');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('select', 'jlo_done',$this->indent . $this->indent . 'Has the JLO been done?', $done_option);

        // JLO Done/Not Done Rules
        $this->XINRegisterRule("jlo_date", array("jlo_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("jlo_type", array("jlo_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("jlo_version", array("jlo_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("jlo_total", array("jlo_done{@}=={@}done"), "Required.");
        


        // JLO Questions
        $this->form->addElement('date', 'jlo_date', $this->indent . $this->indent . 'Date of JLO:');
        $this->form->addElement('select', 'jlo_type',$this->indent .  $this->indent . 'Type of test:', $general_spoken_written_option);


        $this->form->addElement('select', 'jlo_version',$this->indent . $this->indent . 'Test Version:', $jlo_version_option);

        $this->form->addElement('text', 'jlo_total', $this->indent . $this->indent . 'Total score:');
        $this->form->addRule('jlo_total','Numerical value required.', 'numeric');

        // Breaks for formatting
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        
        // Social Behavior Observer Checklist
        $this->form->addElement('static', null, '<font size=4>Social Behavior Observer Checklist</font>');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('select', 'social_behavior_done', $this->indent . $this->indent . 'Has the Social Behavior Observer Checklist been done?', $done_option);

        // Social Behavior Observer Checklist Done/Not Done Rules
        $this->XINRegisterRule("social_behavior_date", array("social_behavior_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("social_behavior_descriptor", array("social_behavior_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("social_behavior_checklist", array("social_behavior_done{@}=={@}done"), "Required.");


        // Social Behavior Observer Checklist Questions
        $this->form->addElement('date', 'social_behavior_date', $this->indent . $this->indent . 'Date:');
                
        $this->form->addElement('text', 'social_behavior_descriptor', $this->indent . $this->indent . 'Descriptor Total score:');
        $this->form->addRule('social_behavior_descriptor','Numerical value required.', 'numeric');

        $this->form->addElement('text', 'social_behavior_checklist', $this->indent . $this->indent . 'Checklist (Behavior) Total Score:');
        $this->form->addRule('social_behavior_checklist','Numerical value required.', 'numeric');
        

        
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
