<?php
class NDB_BVL_Instrument_genetics extends NDB_BVL_Instrument
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
        $this->testName = "genetics";           // test_names.Test_name
        $this->table = 'genetics';             // name of table containing


        // data keyed by commentID
        $this->commentID = $commentID;

        $this->_requiredElements = array('Date_taken', 'Examiner', 'form_type');
        
        $this->dateTimeFields=array("Date_taken", "genetic_date");
        $this->_selectMultipleElements = array("genes", "dementia_options");
        
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
	        $this->form->addElement('header', 'instrument_title', "F<span style='font-size: 26px'>AMILY </span>H<span style='font-size: 26px'>ISTORY &</span> G<span style='font-size: 26px'>ENETIC </span>T<span style='font-size: 26px'>ESTING</span>");
	        $this->form->addElement('static', null, $hr_header);


	        // automatically adds examiner & date of administration
	        $this->_addMetadataFields();
	        $this->form->addElement('static', null, $hr_header);
	        $yes_no_option= array(NULL=>"", "yes"=>"Yes", "no"=>"No", "not_answered"=>"Not Answered");
	    	$normal_option= array(NULL=>"", "initial"=>"Initial","update"=>"Update", "no_change"=>"No Change", "not_answered"=>"Not Answered");
	    	$gene_options = array(NULL=>"", "c9orf72"=>"C9orf72", "sod1"=>"SOD1", "tdp"=>"TDP", "fus"=>"FUS", "other"=>"Other");
	    	$dementia_options =array(NULL=>"", "ftd"=>"FTD", "alzheimers"=>"Alzheimer's Disease", "dementia_lewyBodies"=>"Dementia with Lewy Bodies", "other"=>"Other (Diagnosis other than one listed above)", "unknown"=>"Unknown (diagnosis unknown)");
        // Family History
		
	        $this->form->addElement('select', 'form_type', $this->indent."F<span style='font-size: 12px'>AMILY </span>H<span style='font-size: 12px'>ISTORY</span>", $normal_option);
	        $this->XINRegisterRule("als", array("form_type{@}=={@}initial"), "Required.");
	        $this->XINRegisterRule("mnd", array("form_type{@}=={@}initial"), "Required.");
	        $this->XINRegisterRule("dementia", array("form_type{@}=={@}initial"), "Required.");
	        $this->XINRegisterRule("ftd", array("form_type{@}=={@}initial"), "Required.");
	        $this->XINRegisterRule("alzheimers", array("form_type{@}=={@}initial"), "Required.");
	        $this->XINRegisterRule("psychiatric", array("form_type{@}=={@}initial"), "Required.");
        
        // Horizontal Line Break      

        	$this->form->addElement('static', null, $hr_section);


        // Is there a family history of ALS?

	        $this->form->addElement('static', null, 'Is there a family history of:');
	        $this->form->addElement('select', 'als', $this->indent . $this->indent .'ALS:', $yes_no_option);
	        $this->form->addElement('select', 'mnd', $this->indent . $this->indent . 'Other MND:', $yes_no_option);
	        $this->form->addElement('textarea', 'a_m_other', $this->indent . $this->indent.$this->indent ."<span style='font-weight:normal'><i>If yes, list relation(s) and diagnosis:</i></span>");
	        $this->XINRegisterRule("a_m_other",array("mnd{@}=={@}yes|als{@}=={@}yes"), "Please provide relation(s) and diagnosis.");

        // Horizontal Line Break      

        	$this->form->addElement('static', null, $hr_section);

        // Is there a family history of Dementia?
		
	        $this->form->addElement('select', 'dementia', $this->indent . 'Is there a family history of Dementia:', $yes_no_option);
       		$this->form->addElement('select', 'dementia_options', $this ->indent .$this->indent . "<span style='font-weight:normal'><i>If yes, what types (hold <em><i>ctrl</i></em> to select multiple options):</i></span>", $dementia_options, array('multiple'=>'multiple'));
	       	$this->form->addElement('textarea', 'f_a_other',$this->indent . "<span style='font-weight:normal'><i>If yes, list relation(s) and diagnosis</i></span>");
        	$this->XINRegisterRule("f_a_other",array("dementia{@}=={@}yes"), "Please provide relation(s) and diagnosis.");
			$this->XINRegisterRule("dementia_options", array("dementia{@}=={@}yes"), "Please provide the type(s) of dementia.");

	// Horizontal Line Break      

        	$this->form->addElement('static', null, $hr_section);

        // Is there a family history of psychiatric illness?
        
	        $this->form->addElement('select', 'psychiatric', $this->indent .'Is there a family history of psychiatric illness?', $yes_no_option);
	        $this->form->addElement('textarea', 'p_other', $this->indent . $this->indent ."<span style='font-weight:normal'><i>If yes, list relation(s) and diagnosis</i></span>");
	        $this->XINRegisterRule("p_other",array("psychiatric{@}=={@}yes"), "Please provide relation(s) and diagnosis.");

	    // Horizontal Line Break      

        	$this->form->addElement('static', null, $hr_section);

        // Notes

	        $this->form->addElement('textarea', 'notes_1', $this->indent . 'Notes:');
	        $this->XINRegisterRule("notes_1", array("notes_1{@}=={@}NEVER_REQUIRED"), "Never required.");

        // Horizontal Line Break      

        	$this->form->addElement('static', null, $hr_header);

        // Genetic Testing

	        
	        $this->form->addElement('static', null, '');
	        $this->form->addElement('static', null, '');
	        $this->form->addElement('select', 'completed', $this->indent."G<span style='font-size: 12px'>ENETIC </span>T<span style='font-size: 12px'>ESTING (FROM MEDICAL RECORDS)</span>", array(NULL=>"","done"=>"Done","not_done"=>"Not Done"));
	        $this->XINRegisterRule("genetic_date", array("completed{@}=={@}done"), "Please provide the date of the genetic test.");
	        $this->XINRegisterRule("mutation", array("completed{@}=={@}done"), "Please provide if a mutation was found.");
	        $this->XINRegisterRule("lab_name", array("completed{@}=={@}done"), "Please provide the lab name.");

        // Horizontal Line Break      

        	$this->form->addElement('static', null, $hr_section);

        // genetic date

        	$this->form->addElement('date', 'genetic_date', $this ->indent . 'Date of test:');

        // Horizontal Line Break      

        	$this->form->addElement('static', null, $hr_section);

        // Was a mutation found?

       		$this->form->addElement('select', 'mutation', $this ->indent ."Was a mutation found?", $yes_no_option);

       	// Horizontal Line Break      

        	$this->form->addElement('static', null, $hr_section);

        // Which genes are involved?

	       $this->form->addElement('select', 'genes', $this ->indent . 'Which genes were involved?', $gene_options, array('multiple'=>'multiple'));
	       $this->form->addElement('textarea', 'other_genes', $this ->indent . $this ->indent .$this->indent. "<span style='font-weight:normal'><i>If other, please state:</i></span>");
	        $this->XINRegisterRule("other_genes", array("genes{@}=={@}other"), "Please provide gene(s) that are involved.");
	        $this->XINRegisterRule("genes", array("mutation{@}=={@}yes"), "Please provide the mutation(s).");

	    // Horizontal Line Break      

        	$this->form->addElement('static', null, $hr_section);

        // Genetic sequence

	        $this->form->addElement('text', 'sequence', $this ->indent . 'Genetic Sequences:');
	        $this->XINRegisterRule("sequence", array("mutation{@}=={@}yes"), "Please provide the sequence.");

	    // Horizontal Line Break      

        	$this->form->addElement('static', null, $hr_section);

        // Lab name

	        $this->form->addElement('text', 'lab_name', $this ->indent . 'Lab name:', array('not_known'=>"Unknown"));

	   	// Horizontal Line Break      

        	$this->form->addElement('static', null, $hr_section);

        // Notes 2

	        $this->form->addElement('textarea', 'note_2', $this ->indent . 'Notes:');
	        $this->XINRegisterRule("note_2",array("note_2{@}=={@}NEVER_REQUIRED"),"Not required");



        
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
