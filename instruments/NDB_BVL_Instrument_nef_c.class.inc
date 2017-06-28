<?php
class NDB_BVL_Instrument_nef_c extends NDB_BVL_Instrument
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
        $this->testName = "nef_c";           // test_names.Test_name
        $this->table = 'nef_c';             // name of table containing


        // data keyed by commentID
        $this->commentID = $commentID;

        $this->_requiredElements = array('Date_taken', 'Examiner');
        
        $this->dateTimeFields=array("Date_taken", "tapping_date", "speech_date","saccades_date","mri_date");
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
        $this->form->addElement('header', 'instrument_title', "Neurological Evaluations Form");
       $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');


        // automatically adds examiner & date of administration
        $this->_addMetadataFields();

        $yes_no_option= array(NULL=>"", "yes"=>"Yes", "no"=>"No", "not_answered"=>"Not Answered");
    	$tapping_option= array(NULL=>"", "done"=>"Done","not_done"=>"Not Done", "unable"=>"Unable");
        $done_option=array(NULL=>"", "done"=>"Done","not_done"=>"Not Done");
    	
        // Tapping
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
            
        $this->form->addElement('static', null, '<font size=4>Tapping</font>');
         $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('select', 'tapping_done',$this->indent . $this->indent . 'Has the Tapping section been done?', $tapping_option);

        $this->XINRegisterRule("f_r_1", array("tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("f_l_1", array("tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("f_r_2", array("tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("f_l_2", array("tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("fe_r_1", array("tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("fe_l_1", array("tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("fe_r_2", array("tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("fe_l_2", array("tapping_done{@}=={@}done"), "Required.");


            //Fingers
        $this->form->addElement('static', null, $this->indent . '<font size=3>Fingers</font>');

        $this->form->addElement('static', null, $this->indent . $this->indent . $this->indent . '<i>Trial 1</i>');
        $this->form->addElement('text', 'f_r_1', $this->indent . $this->indent . $this->indent . $this->indent . $this->indent . 'Right');
        $this->form->addRule('f_r_1','Numerical value required.', 'numeric');
        $this->form->addElement('text', 'f_l_1', $this->indent . $this->indent . $this->indent . $this->indent . $this->indent . 'Left');
        $this->form->addRule('f_l_1','Numerical value required.', 'numeric');

         $this->form->addElement('static', null, $this->indent . $this->indent . $this->indent . '<i>Trial 2</i>');
        $this->form->addElement('text', 'f_r_2', $this->indent . $this->indent . $this->indent . $this->indent . $this->indent . 'Right');
        $this->form->addRule('f_r_2','Numerical value required.', 'numeric');
        $this->form->addElement('text', 'f_l_2', $this->indent . $this->indent . $this->indent . $this->indent . $this->indent . 'Left');
        $this->form->addRule('f_l_2','Numerical value required.', 'numeric');

            //Feet
        $this->form->addElement('static', null, $this->indent . '<font size=3>Feet</font>');

        $this->form->addElement('static', null, $this->indent . $this->indent . '<i>Trial 1</i>');
        $this->form->addElement('text', 'fe_r_1', $this->indent . $this->indent . $this->indent . $this->indent . 'Right');
        $this->form->addRule('fe_r_1','Numerical value required.', 'numeric');
        $this->form->addElement('text', 'fe_l_1', $this->indent . $this->indent . $this->indent . $this->indent . 'Left');
        $this->form->addRule('fe_l_1','Numerical value required.', 'numeric');

         $this->form->addElement('static', null, $this->indent . $this->indent . '<i>Trial 2</i>');
        $this->form->addElement('text', 'fe_r_2', $this->indent . $this->indent . $this->indent . $this->indent . 'Right');
        $this->form->addRule('fe_r_2','Numerical value required.', 'numeric');
        $this->form->addElement('text', 'fe_l_2', $this->indent . $this->indent . $this->indent . $this->indent . 'Left');
        $this->form->addRule('fe_l_2','Numerical value required.', 'numeric');

        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');

        // Speech Testing
        $this->form->addElement('static', null, '<font size=4>Speech Testing</font>');
        $this->form->addElement('select', 'speech_done',$this->indent .  $this->indent . 'Has Speech Testing been done?', $done_option);
        $this->form->addElement('date', 'speech_date', $this->indent . $this->indent . 'Date of Speech Testing:');
        $this->form->addElement('text', 'file_number', $this->indent . $this->indent . 'SIT file number:');
        $this->form->addElement('text', 'file_name', $this->indent . $this->indent . 'Audio file name:');
        $this->XINRegisterRule("speech_date", array("speech_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("file_number", array("speech_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("file_name", array("speech_done{@}=={@}done"), "Required.");

        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');

        // Saccades
        $this->form->addElement('static', null, '<font size=4>Saccades / Antisaccades</font>');
        $this->form->addElement('select', 'saccades_done', $this->indent . $this->indent . 'Has the Saccades/Antisaccades section been done?', $done_option);
        $this->form->addElement('date', 'saccades_date', $this->indent . $this->indent . 'Date of Saccades/Antisaccades Testing:');
        $this->form->addElement('text', 'saccades', $this->indent . $this->indent . '# Correct Saccades (/8):');
        $this->form->addRule('saccades','Numerical value required.', 'numeric');
        $this->form->addElement('text', 'antisaccades', $this->indent . $this->indent . '# Correct Antisaccades (/8):');
        $this->form->addRule('antisaccades','Numerical value required.', 'numeric');
        $this->XINRegisterRule("saccades_date", array("saccades_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("saccades", array("saccades_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("antisaccades", array("saccades_done{@}=={@}done"), "Required.");
 
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');        
        // Research MRi
        $this->form->addElement('static', null, '<font size=4>Research MRI</font>');
        $this->form->addElement('select', 'mri_done', $this->indent . $this->indent . 'Has the MRI been done?', $done_option);
        $this->form->addElement('date', 'mri_date', $this->indent . $this->indent . 'Date of MRI:');
        $this->form->addElement('text', 'mri_filename', $this->indent . $this->indent . 'MRI file name:');
        $this->form->addElement('select', 'medication', $this->indent . $this->indent . 'Medication taken prior to scan (for claustrophobia, excess saliva etc.)?', $yes_no_option);
        $this->form->addElement('textarea', 'medication_detail', $this->indent . $this->indent . $this->indent .'If yes, indicate medication, dose, time:');
        $this->form->addElement('textarea', 'notes', $this->indent . $this->indent .'Notes:');
        $this->XINRegisterRule("mri_filename", array("mri_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("medication", array("mri_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("mri_date", array("mri_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("medication_detail", array("medication{@}=={@}yes"), "Please provide medication details.");
        $this->XINRegisterRule("notes",array("notes{@}=={@}NEVER_REQUIRED"),"Not required");

        

        
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
        if (strrpos($this->testName, '_proband') === false) {
            $this->addScoreColumn(
                'Candidate_Age',
                'Candidate Age (Months)',
                'Please enter date of administration and press Save'
            );
            $this->addScoreColumn(
                'Window_Difference',
                'Window Difference (+/- Days)',
                'Please enter date of administration and press Save'
            );
        }
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
