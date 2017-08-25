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
    	$tapping_option= array(NULL=>"", "done"=>"Done","not_done"=>"Not Done");
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
        $this->form->addElement('date', 'tapping_date', $this->indent . $this->indent . 'Date of Tapping:');

        $this->XINRegisterRule("f_r_1", array("tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("f_l_1", array("tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("f_r_2", array("tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("f_l_2", array("tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("fe_r_1", array("tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("fe_l_1", array("tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("fe_r_2", array("tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("fe_l_2", array("tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("tapping_date", array("tapping_done{@}=={@}done"), "Required.");

            //Fingers
        $this->form->addElement('static', null, $this->indent . '<font size=3>Fingers </font><i><font size=2>(If unable, enter "0")  </font></i>.');

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
        $this->form->addElement('static', null, $this->indent . '<font size=3>Feet </font><i><font size=2>(If unable, enter "0")  </font></i>.');

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
        $this->form->addElement('file', 'upload_file', $this->indent . $this->indent .'Upload the speech testing file');
        $this->XINRegisterRule("upload_file", array("speech_done{@}=={@}NOT_REQUIRED"), "Required.");
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
    function _saveValues($values)
    {
         $timepoint = TimePoint::singleton($this->getSessionID());
         $candidate = Candidate::singleton($timepoint->getCandID());
         if (isset($values['upload_file']) && !empty($values['upload_file']['name'])) {
            include_once "File_Upload.class.inc";
            $file=new File_Upload;
            //pass the existing form ($form) by reference to File_Upload, and register the file_upload field names being used.
            $file->registerForm($this->form);
            //Tell File_Upload what file handlers to use.
            $file->setFileHandler("upload_file", $this);
            //Set the target directory that you want files moved into once they are validated and processed.
            $config = NDB_Config::singleton();
            $dir = "/data/uploads/";
            
            if (empty($dir)) {
                $dir = ".";
            };

            $file->setBaseUploadDirectory($dir);
            //Set the prefix to prepend to the filenames
            //$file->setFilenamePrefix($timepoint->getVisitLabel()."-".$this->testName."-");
            //set the the IDs to the handler functions.
            $user =& User::singleton();
            $file->setHandlerArgs(
                array("CommentID"=>$this->getCommentID(),
                       "candID"=>$candidate->getCandID(),
                       "PSCID"=>$candidate->getPSCID(),
                       "visitLabel"=>$timepoint->getVisitLabel(),
                       "username"=>$user->getUsername(),
                       "values"=>$values)
            );
            
            //If the form is validated, call File_Upload::processFiles() which loops through the files and 
            //proccesses them (including verify, move, and import steps)
            $file->processFiles();
        } else {
            $this->form->setElementError(
                    'mainError',
                    "<p>
                    <font color='red'>
                        No <b>speech testing file</b> was selected for upload, however data has been saved.
                    </font>
                    </p>\n"
                );
        }
        //print_r($file);
        //echo error messages

        if(!empty($file->errorLog)) {
            while(list($fileType,$fileErrors)=each($file->errorLog)){
                foreach($fileErrors AS $error){
                     $this->form->setElementError(
                    'mainError',
                    "<p>
                    <font color='red'>
                        <b>Upload Error</b> - $error File was not uploaded, however data has been saved.
                    </font>
                    </p>\n"
                );
                }
            }
        }
        
        unset(
            $values['candID'], $values['sessionID'], $values['commentID'], $values['test_name'],
            $values['page'], $values['fire_away'], $values['subtest'], $values['MAX_FILE_SIZE'], 
            $values['upload_file']
        );
        
        $this->_save($values);
        
    }
    /**
     * getTargetDirectory retrieves info about where the file should be stored.
     *
     * The returned directory is ADDED to the baseUploadDirectory proprety.  It should have a trailing slash.
     *
     * @param string $file The full filename including directory.
     *
     * @return bool    $success     if operation succeeded
     * @access public
     */

    function getTargetDirectory(&$file, $args)
    {
        
        $output="";
        
        return $output;
    }

    /*
    * Import data from the uploaded file
    *
    * @param $file Reference to file object
    * @param $args An associative array set by setHandlerArgs function
    *
    * @return true If operation succeeded
    */
    
    function importFile(&$file, $args) 
    {
        $fp=fopen($file->fileInfo['tmp_name'], "r");
        $db=& Database::singleton();
        ///Setting trackchanges to false because getting error messages
        $db->_trackChanges = false;
        ////////////////////////////////////////////////////////////////
        $upload_values = array(); //uploading values to <TEST_NAME> table
        /*parse the file and store the values to upload in the array above
        * and save to the database
        */
        // update database
   
        $media_query = array (
                        "session_id"      => $this->getSessionID(),
                        "instrument"      => $this->testName,
                        "date_taken"      => $args['values']['speech_date'],
                        "file_name"       => $args['values']['upload_file']['name'],
                        "file_type"       => $args['values']['upload_file']['type'],
                        "data_dir"        => "/data/uploads/",
                        "uploaded_by"     => $args['username'],
                        "hide_file"       => 0             
                        );
     
        //$result = $db->update($this->testName, $upload_values, array('CommentID'=>$args['CommentID']));
        $result = $db->insert('media',$media_query);

        echo "Upload Successful : ".$args['values']['upload_file']['name'];
        return true;
    }

    /**
    * isValid checks the validity of the file.
    *
    * @param     string  $file    The full filename including directory.
    *
    * @return    bool    $success     if operation succeeded
    * @access    public
    */
    function isValid(&$file, $args){

        $db=& Database::singleton();
        $timepoint = TimePoint::singleton($this->getSessionID());
        $candidate = Candidate::singleton($timepoint->getCandID());
        $PSCID = $candidate->getPSCID();
        $file_name = explode("_",$args['values']['upload_file']['name']);
        $file_type = explode("_",$args['values']['upload_file']['type']);

        if ($PSCID != $file_name[0]){
            
            
            return array("Improper format: File name does not begin with PSCID.");
        }
       
     
        if (strpos($file_type[0],"audio")=== false && strpos($file_type[0],"video")=== false){
            return array("Improper format: File is not an audio type. $file_type[0]");
        }
        $uploadedFile= pathinfo($args['values']['upload_file']['name']);

        if ($uploadedFile['filename'] !== $args['values']['file_name']){
            return array("Improper format: File name does not match the speech file name that was provided.");
        }
        $media_files = array_column($db->pselect("SELECT file_name FROM media", []),'file_name');
        
        if (in_array(strtolower($uploadedFile['basename']),array_map('strtolower',$media_files))){
            return array("A file with the same name has already been uploaded!");
        }
        return true;
    }
}
?>
