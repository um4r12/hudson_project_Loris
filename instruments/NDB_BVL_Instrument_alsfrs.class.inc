<?php
class NDB_BVL_Instrument_alsfrs extends NDB_BVL_Instrument
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
        $this->testName = "alsfrs";           // test_names.Test_name
        $this->table = 'alsfrs';             // name of table containing


        // data keyed by commentID
        $this->commentID = $commentID;

        $this->_requiredElements = array('Date_taken', 'Examiner');
        
        $this->dateTimeFields=array("Date_taken");
        
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
        $this->form->addElement('header', 'instrument_title', "A<span style='font-size: 26px'>LSFRS-R </span>F<span style='font-size: 26px'>ORM</span>");
        $this->form->addElement('static', null, $hr_header);
      
        // automatically adds examiner & date of administration
        $this->_addMetadataFields();
        $this->form->addElement('static', null, $hr_header);

        $yes_no_option= array(NULL=>"", "Yes"=>"Yes", "No"=>"No");
        $scores =array(NULL=>"","0"=>"0","1"=>"1","2"=>"2","3"=>"3","4"=>"4");

    	


    // ALSFRS-R FORM

        // Gastrostomy 

            $this->form->addElement('select', 'gastrostomy', $this->indent."H<span style='font-size: 12px'>AS </span>P<span style='font-size: 12px'>ATIENT </span>H<span style='font-size: 12px'>AD </span>G<span style='font-size: 12px'>ASTROSTOMY (PEG TUBE)</span>", $yes_no_option);

            $this->form->addElement('static', null, $hr_section);


        // Speech

            // Entry for Speech Score

                $this->form->addElement('select', 'speech', $this->indent . '1. Speech Score:',$scores);

            // Rule: Speech Score must be a numerical value.

                $this->form->addRule('speech','Enter a numerical value between 0 and 4', 'numeric',0,4);

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Salivation

            // Entry for Salivation Score

                $this->form->addElement('select', 'salivation', $this->indent . '2. Salivation Score:',$scores);

            // Rule: Salivation Score must be a numerical value.

                $this->form->addRule('salivation','Enter a numerical value between 0 and 4', 'numeric',0,4);
                
        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        // Swallowing

            // Entry for Swallowing Score

                $this->form->addElement('select', 'swallowing', $this->indent . '3. Swallowing Score:',$scores);

            // Rule: Swallowing Score must be a numerical value.

                $this->form->addRule('swallowing','Enter a numerical value between 0 and 4', 'numeric',0,4);
                
        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        // Handwriting

            // Entry for Handwriting Score

                $this->form->addElement('select', 'handwriting', $this->indent . '4. Handwriting Score:',$scores);

            // Rule: Handwriting Score must be a numerical value.

                $this->form->addRule('handwriting','Enter a numerical value between 0 and 4', 'numeric',0,4);
                
        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        // Cutting Food and Handling Utensils

            // Entry for Cutting Food and Handling Utensils (without gastrostomy) Score

                $this->form->addElement('select', 'cutting_no_gastrostomy', $this->indent . '5a. Cutting food and handling utensils (patients WITHOUT gastrostomy):',$scores);

            // Rule: Cutting Food and Handling Utensils (without gastrostomy) Score must be a numerical value.

                $this->form->addRule('cutting_no_gastrostomy','Enter a numerical value between 0 and 4', 'numeric',0,4);

            // Entry for Cutting Food and Handling Utensils (with gastrostomy) Score

                $this->form->addElement('select', 'cutting_gastrostomy', $this->indent . '5b. Cutting food and handling utensils (patients WITH gastrostomy):',$scores);

            // Rule: Cutting Food and Handling Utensils (with gastrostomy) Score must be a numerical value.

                $this->form->addRule('cutting_gastrostomy','Enter a numerical value between 0 and 4', 'numeric',0,4);


            // Rule: Entry for Cutting Food and Handling Utensils (without gastrostomy) Score is required if Gastrostomy is set to no.

                $this->XINRegisterRule("cutting_no_gastrostomy",array("gastrostomy{@}=={@}No"),"This is required if patient has not had gastrostomy done.");

             // Rule: Entry for Cutting Food and Handling Utensils (with gastrostomy) Score is required if Gastrostomy is set to yes.

                $this->XINRegisterRule("cutting_gastrostomy",array("gastrostomy{@}=={@}Yes"),"This is required if patient has had gastrostomy done.");
                
        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        // Dressing & Hygiene

            // Entry for Dressing & Hygiene Score

                $this->form->addElement('select', 'dressing', $this->indent . '6. Dressing & Hygiene Score:',$scores);

            // Rule: Dressing & Hygiene Score must be a numerical value.

                $this->form->addRule('dressing','Enter a numerical value between 0 and 4', 'numeric',0,4);
                
        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Turning in Bed and Adjusting Bed Clothes

            // Entry for Bed and Adjusting Bed Clothes Score

                $this->form->addElement('select', 'turning', $this->indent . '7. Bed and Adjusting Bed Clothes:',$scores);

            // Rule: Bed and Adjusting Bed Clothes Score must be a numerical value.

                $this->form->addRule('turning','Enter a numerical value between 0 and 4', 'numeric',0,4);
                
        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Walking

            // Entry for Walking Score

                $this->form->addElement('select', 'walking', $this->indent . '8. Walking:',$scores);

            // Rule: Walking Score must be a numerical value.

                $this->form->addRule('walking','Enter a numerical value between 0 and 4', 'numeric',0,4);
                
        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        // Climbing Stairs

            // Entry for Climbing Stairs Score

                $this->form->addElement('select', 'climbing', $this->indent . '9. Climbing Stairs:',$scores);

            // Rule: Climbing Stairs Score must be a numerical value.

                $this->form->addRule('climbing','Enter a numerical value between 0 and 4', 'numeric',0,4);
                
        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Dyspnea

            // Entry for Dyspnea Score

                $this->form->addElement('select', 'dyspnea', $this->indent . '10. Dyspnea:',$scores);

            // Rule: Dyspnea Score must be a numerical value.

                $this->form->addRule('dyspnea','Enter a numerical value between 0 and 4', 'numeric',0,4);
                
        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        // Orthopnea

            // Entry for Orthopnea Score

                $this->form->addElement('select', 'orthopnea', $this->indent . '11. Orthopnea:',$scores);

            // Rule: Orthopnea Score must be a numerical value.

                $this->form->addRule('orthopnea','Enter a numerical value between 0 and 4', 'numeric',0,4);
                
        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        // Respiratory Insufficiency

            // Entry for Respiratory Insufficiency Score

                $this->form->addElement('select', 'respiratory', $this->indent . '12. Respiratory Insufficiency:',$scores);

            // Rule: Respiratory Insufficiency Score must be a numerical value.

                $this->form->addRule('respiratory','Enter a numerical value between 0 and 4', 'numeric',0,4);
                
        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

         // Automated Total Score

            // Field for Total Score

                $this->form->addElement('static', 'total', $this->indent . 'Total Score:');


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