<?php
class NDB_BVL_Instrument_fvc extends NDB_BVL_Instrument
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
        $this->testName = "fvc";           // test_names.Test_name
        $this->table = 'fvc';             // name of table containing


        // data keyed by commentID
        $this->commentID = $commentID;

        $this->_requiredElements = array('Date_taken', 'Examiner');
        
        $this->dateTimeFields=array("Date_taken");
        $this->_selectMultipleElements = array("breathing_conditions");
        
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
     * Wrapper to create a text field for height with an accompanying scale field.
     * Includes rule for text field to only accept numerical values
     * Prints out error message specific to the error made:
     *      If height not entered, will ask you to enter height.
     *      If height entered, but it is not a numerical value, will ask you to enter a numerical value.
     *      If scale not selected, will ask you to select scale.   
     */
    function addHeightElement(
        $field,
        $label,
        $rules=array(),
        $height_message='Provide the height.',
        $scale_message='Provide the measurement scale in the drop-down.',
        $refusals = array(
                     NULL             => '',
                     'cm'             => 'Centimeters',
                     'inches' => 'Inches',
                    )
    ) {
        $group[] = $this->createText($field, $label);
        $this->WrapperTextElements[$field] = $group[0];
        $group[] = $this->createSelect(
            $field."_scale",
            "",
            $refusals,
            array('class' => 'form-control input-sm')
        );

        $this->addGroup($group, $field.'_group', $label, null, false);
        unset($group);

        $rule = 'test_type{@}=={@}Forced Vital Capacity|Slow Vital Capacity';
        
        //$scale_array = array_merge(array($field.'_scale{@}=={@}'), $rules);
        //$height_array = array_merge(array($field.'{@}=={@}'), $rules);

        $this->form->addGroupRule($field."_group",array(array(array("Value must be numeric", 'numeric'))));
        $this->XINRegisterRule($field, $rule, $height_message, $field.'_group');
        $this->XINRegisterRule($field.'_scale', $rule, $scale_message, $field.'_group');

    }
    /**
     * Wrapper to create a text field for weight with an accompanying scale field.
     * Includes rule for text field to only accept numerical values
     * Prints out error message specific to the error made:
     *      If weight not entered, will ask you to enter weight.
     *      If weight entered, but it is not a numerical value, will ask you to enter a numerical value.
     *      If scale not selected, will ask you to select scale.   
     */
    function addWeightElement(
        $field,
        $label,
        $rules=array(),
        $weight_message='Provide the weight.',
        $scale_message='Provide the measurement scale in the drop-down.',
        $refusals = array(
                     NULL             => '',
                     'kg'             => 'Kilograms',
                     'lbs' => 'Pounds',
                    )
    ) {
        $group[] = $this->createText($field, $label);
        $this->WrapperTextElements[$field] = $group[0];
        $group[] = $this->createSelect(
            $field."_scale",
            "",
            $refusals,
            array('class' => 'form-control input-sm')
        );

        $this->addGroup($group, $field.'_group', $label, null, false);
        unset($group);
        $rule = 'test_type{@}=={@}Forced Vital Capacity|Slow Vital Capacity';
        //$scale_array = array_merge(array($field.'_scale{@}=={@}'), $rules);
        //$weight_array = array_merge(array($field.'{@}=={@}'), $rules);
      


        $this->form->addGroupRule($field."_group",array(array(array("Value must be numeric", 'numeric'))));
        $this->XINRegisterRule($field, $rule, $weight_message, $field.'_group');
        $this->XINRegisterRule($field.'_scale', $rule, $scale_message, $field.'_group');
       
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
        $hr_section2 = "<hr style='height:2px; border:none; color:rgb(58,106,117); margin-top: 0px; background-color:rgb(58,106,117)'; align='left'; width='1000px'>";

        //Defines the call back function for HTML Quickform to use when validating the form.
        $this->form->addFormRule(array(&$this,'XINValidate'));

        // display test name
        $this->form->addElement('header', 'instrument_title', "F<span style='font-size: 26px'>ORCED </span>V<span style='font-size: 26px'>ITAL </span>C<span style='font-size: 26px'>APACITY </span>F<span style='font-size: 26px'>ORM </span>");
        $this->form->addElement('static', null, $hr_header);
      
        // automatically adds examiner & date of administration

        $this->_addMetadataFields();

        $this->form->addElement('static', null, $hr_header);

        $test_type = array(NULL=>"","Forced Vital Capacity"=>"Forced Vital Capacity","Slow Vital Capacity"=>"Slow Vital Capacity","Not Done"=>"Not Done");
        $ethnicity = array(NULL=>"","Caucasian"=>"Caucasian","Asian"=>"Asian","Aboriginal"=>"Aboriginal","Hispanic"=>"Hispanic","Black or African-Canadian"=>"Black or African-Canadian", "Other"=>"Other");
        $reference_set = array(NULL=>"","Knudsen 83"=>"Knudsen 83","CRAPO"=>"CRAPO","NHANES III"=>"NHANES III","Canadian Ref 04"=>"Canadian Ref 04","ERS/ECCS"=>"ERS/ECCS","Other"=>"Other");
        $height = array(NULL=>"","cm"=>"Centimeters","inches"=>"Inches");
        $weight = array(NULL=>"","kg"=>"Kilograms","lbs"=>"Pounds");
        $breathing_conditions = array(NULL=>"","None"=>"None","Asthma"=>"Asthma","Possible Asthma"=>"Possible Asthma","COPD"=>"COPD","Pneumonia"=>"Pneumonia","Other"=>"Other");
        $smoker = array(NULL=>"","Yes"=>"Yes","No"=>"No","Former"=>"Former");
        $fvc_reliability= array(NULL=>"", "Reliable"=>"Reliable", "Unreliable"=>"Unreliable","Not Available"=>"Not Available","Unknown"=>"Unknown");

    	


    // Forced Vital Capacity Form

        // Type of Test 

            $this->form->addElement('select', 'test_type', $this->indent."T<span style='font-size: 12px'>YPE OF </span>T<span style='font-size: 12px'>EST </span>", $test_type);

            $this->form->addElement('static', null, $hr_section2);


        // Trial One

            // Entry for Trial One Volume

                $this->form->addElement('text', 'trial_one', $this->indent . 'Trial 1 - Volume (L):');

            // Rule: Trial One must be a numerical value.

                $this->form->addRule('trial_one','Enter a numerical value.', 'numeric');

            // Rule: Entry of Trial One is required only if FVC or SVC was done.

                $this->XINRegisterRule("trial_one", array("test_type{@}=={@}Forced Vital Capacity|Slow Vital Capacity"), "Required.");

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        // Trial Two

            // Entry for Trial Two Volume

                $this->form->addElement('text', 'trial_two', $this->indent . 'Trial 2 - Volume (L):');

            // Rule: Trial Two must be a numerical value.

                $this->form->addRule('trial_two','Enter a numerical value.', 'numeric');


	    // Rule: Entry of Trial Two is never required.

                $this->XINRegisterRule("trial_two", array("trial_two{@}=={@}NEVER_REQUIRED"), "Not required.");

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        // Trial Three

            // Entry for Trial Three Volume

                $this->form->addElement('text', 'trial_three', $this->indent . 'Trial 3 - Volume (L):');

            // Rule: Trial Three must be a numerical value.

                $this->form->addRule('trial_three','Enter a numerical value.', 'numeric');

            // Rule: Entry of Trial Three is never required.

                $this->XINRegisterRule("trial_three", array("trial_three{@}=={@}NEVER_REQUIRED"), "Not required.");

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        // Trial Four

            // Entry for Trial Four Volume

                $this->form->addElement('text', 'trial_four', $this->indent . "Trial 4 - Volume (L) <span style='font-weight:normal'><i>(If trials 1-3 > 10% variability)</i></span> :");

            // Rule: Trial Four must be a numerical value.

                $this->form->addRule('trial_three','Enter a numerical value.', 'numeric');

            // Rule: Entry of Trial Four is never required.

                $this->XINRegisterRule("trial_four", array("trial_four{@}=={@}NEVER_REQUIRED"), "Not required.");

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        // Trial Five

            // Entry for Trial Five Volume

                $this->form->addElement('text', 'trial_five', $this->indent . "Trial 5 - Volume (L) <span style='font-weight:normal'><i>(If trials 1-4 > 10% variability)</i></span> :");

            // Rule: Trial Five must be a numerical value.

                $this->form->addRule('trial_five','Enter a numerical value.', 'numeric');

            // Rule: Entry of Trial Five is never required.

                $this->XINRegisterRule("trial_five", array("trial_five{@}=={@}NEVER_REQUIRED"), "Not required.");


        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        // Best Trial Volume

            // Entry for Best Trial Volume

                $this->form->addElement('text', 'best_trial_volume', $this->indent . "Best Trial (L):");

            // Rule: Best Trial Volume must be a numerical value.

                $this->form->addRule('best_trial_volume','Enter a numerical value.', 'numeric');

            // Rule: Entry of Best Trial Volume is required only if FVC or SVC was done.

                $this->XINRegisterRule("best_trial_volume", array("test_type{@}=={@}Forced Vital Capacity|Slow Vital Capacity"), "Required.");

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        // Best Trial Percentage

            // Entry for Best Trial Percentage

                $this->form->addElement('text', 'best_trial_percentage', $this->indent . "Best Trial (% predicted):");

            // Rule: Best Trial must be a numerical value.

                $this->form->addRule('best_trial_percentage','Enter a numerical value.', 'numeric');

            // Rule: Entry of Best Trial Percentage is required only if FVC or SVC was done.

                $this->XINRegisterRule("best_trial_percentage", array("test_type{@}=={@}Forced Vital Capacity|Slow Vital Capacity"), "Required.");

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        // Ethnicity

            // Entry for Ethnicity

                $this->form->addElement('select', 'ethnicity', $this->indent . "Ethnicity:", $ethnicity);

            // Entry for Other Ethnicity

                $this->form->addElement('text', 'other_ethnicity', $this->indent . $this->indent. "<span style='font-weight:normal'><i>If other, state:</i></span>");

            // Rule: Entry of Ethnicity is required only if FVC or SVC was done.

                $this->XINRegisterRule("ethnicity", array("test_type{@}=={@}Forced Vital Capacity|Slow Vital Capacity"), "Required.");

            // Rule for Other Ethnicity required only if Ethnicity is set to 'Other'

                $this->XINRegisterRule("other_ethnicity",array("ethnicity{@}=={@}Other"),"Please provide an ethnicity.");

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Predicted Reference

            // Entry for Predicted Reference

                $this->form->addElement('select', 'predicted_reference', $this->indent . "Predicted reference set used:", $reference_set);

            // Entry for Other Predicted Reference

                $this->form->addElement('text', 'other_predicted_reference', $this->indent . $this->indent. "<span style='font-weight:normal'><i>If other, state:</i></span>");

            // Rule: Entry of Predicted Reference is required only if FVC or SVC was done.

                $this->XINRegisterRule("predicted_reference", array("test_type{@}=={@}Forced Vital Capacity|Slow Vital Capacity"), "Required.");

            // Rule for Other Predicted Reference required only if Predicted Reference is set to 'Other'

                $this->XINRegisterRule("other_predicted_reference",array("predicted_reference{@}=={@}Other"),"Please provide a predicted reference set.");

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        // Height

            $this->addHeightElement('height', $this->indent . "Height:" );

        // Horizontal Line Break

            //$this->form->addElement('static', null, $hr_section);
         
        // Height Automated Cm

        	//$this->form->addElement('static', 'height_automated_cm', $this->indent . "Height Automated Centimeters:" );

        // Height Automated Inches

        	//$this->form->addElement('static', 'height_automated_inches', $this->indent . "Height Automated Inches:" );

        // Weight Automated Kg

        	//$this->form->addElement('static', 'weight_automated_kgs', $this->indent . "Weight Automated Kilograms:" );

        // Height Automated Lbs

        	//$this->form->addElement('static', 'weight_automated_lbs', $this->indent . "Weight Automated Pounds:" );	

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        // Weight

            $this->addWeightElement('weight', $this->indent . "Weight:" );
        
        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);

        // Breathing Conditions

            // Entry for Breathing Conditions

                $this->form->addElement('select', 'breathing_conditions', $this->indent . "Breathing Conditions:", $breathing_conditions, array("multiple"=>"multiple"));

            // Entry for Other Breathing Conditions

                $this->form->addElement('text', 'other_breathing_conditions', $this->indent . $this->indent. "<span style='font-weight:normal'><i>If other, state:</i></span>");

            // Rule: Entry of Breathing Conditions is required only if FVC or SVC was done.

                $this->XINRegisterRule("breathing_conditions", array("test_type{@}=={@}Forced Vital Capacity|Slow Vital Capacity"), "Required.");

            // Rule for Other Breathing Conditions required only if Breathing Reference is set to 'Other'

                $this->XINRegisterRule("other_breathing_conditions",array("breathing_conditions{@}=={@}Other"),"Please provide other breating conditions(s), each separated by a comma.");

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Smoker

            // Entry for Smoker

                $this->form->addElement('select', 'smoker', $this->indent . "Smoker:", $smoker);

            // Rule: Entry of Smoker is required only if FVC or SVC was done.

                $this->XINRegisterRule("smoker", array("test_type{@}=={@}Forced Vital Capacity|Slow Vital Capacity"), "Required.");

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // FVC Reliability

            // Entry for FVC Reliability

                $this->form->addElement('select', 'fvc_reliability', $this->indent . "FVC Reliability:", $fvc_reliability);

            // Rule: Entry of FVC Reliability is required only if FVC or SVC was done.

                $this->XINRegisterRule("fvc_reliability", array("test_type{@}=={@}Forced Vital Capacity|Slow Vital Capacity"), "Required.");

        // Horizontal Line Break

            $this->form->addElement('static', null, $hr_section);


        // Notes

            // Entry for Notes

                $this->form->addElement('textarea', 'notes', $this->indent . "Notes:");

            // Rule: Entry of Notes is never required.

                $this->XINRegisterRule("notes", array("notes{@}=={@}NEVER_REQUIRED"), "Not required.");





            





    } // End Setup form

    /**
	* scoring function
	*
	* @return void
	*/
	function score() {
	    //if ($this->_determineDataEntryCompletionStatus() == "Incomplete") {
	    //    return;
	    //}
	        
	    $db =& Database::singleton();
	    // Get raw questions point values
	    $query = "SELECT * FROM " . $this->table .
	        " WHERE CommentID='" . $this->getCommentID() . "'";
	    $record = $db->pselectRow($query, null);
	    //print_r($record);

	    // Initializing variables in case there is an error calculating 
	    $height_cm=null;
	   	$height_inches=null;
	   	$weight_kg=null;
	   	$weight_lbs=null;

	    // Automate inches or cm 

	    	// If value entered is inches

			    if ($record['height_scale']=='inches'){
			    	if (is_numeric($record['height'])) {

			    		$height_cm = $record['height']*2.54;
			    		$height_inches = $record['height'];
			    	}
	    	// If value entered is cm

			    } elseif ($record['height_scale']=='cm'){
			    	if (is_numeric($record['height'])) {

			    		$height_cm = $record['height'];
			    		$height_inches = $record['height']/2.54;
			    	
			    	}
			    }
			    
		// Automate kg or lbs 

	    	// If value entered is kg

			    if ($record['weight_scale']=='kg'){
			    	if (is_numeric($record['weight'])) {

			    		$weight_lbs = $record['weight']*2.2;
			    		$weight_kg = $record['weight'];
			    	}

	    	// If value entered is lbs

			    } elseif ($record['weight_scale']=='lbs'){
			    	if (is_numeric($record['weight'])) {

			    		$weight_lbs = $record['weight'];
			    		$weight_kg = $record['weight']/2.2;
			    	}
			    }
	    $score = array('height_automated_cm' => round($height_cm,2),'height_automated_inches' => round($height_inches,2), 'weight_automated_kg'=>round($weight_kg,2), 'weight_automated_lbs'=>round($weight_lbs,2));
	        
	 

	    // save the scores
	    $db->update($this->table,$score,array('CommentID'=>$this->getCommentID()));
	}
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
