<?php
class NDB_BVL_Instrument_nef_p extends NDB_BVL_Instrument
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
        $this->testName = "nef_p";           // test_names.Test_name
        $this->table = 'nef_p';             // name of table containing


        // data keyed by commentID
        $this->commentID = $commentID;

        $this->_requiredElements = array('Date_taken', 'Examiner');
        
        $this->dateTimeFields=array("Date_taken", "tapping_date", "speech_date","saccades_date","mri_date", 'exam_date', 'emg_date');
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
        $this->form->addElement('header', 'nef_p', "Neurological Evaluations Form");
       $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');


        // automatically adds examiner & date of administration
        $this->_addMetadataFields();

        $yes_no_option= array(NULL=>"", "yes"=>"Yes", "no"=>"No", "not_answered"=>"Not Answered");
    	$yes_no_unable_option = array(NULL=>"", "yes"=>"Yes", "no"=>"No", "unable"=>"Unable", "not_answered"=>"Not Answered");
	$tapping_option= array(NULL=>"", "done"=>"Done","not_done"=>"Not Done");
        $done_option=array(NULL=>"", "done"=>"Done","not_done"=>"Not Done");
        $elescorial_option=array(NULL=>"", "suspected"=>"Suspected","possible"=>"Possible", 'probable'=>'Probable','probable_lab'=>'Porbable lab supported','definite'=>'Definite','not_available'=>'N/A');
        $diagnosis_option=array(NULL=>"", "als"=>"ALS","als_ftd"=>"ALS-FTD", 'ftd'=>'FTD','pls'=>'PLS','pma'=>'PMA','msa'=>'MSA','hsp'=>'HSP','kennedy'=>"Kennedy's",'spinocerebellar_ataxia'=>'Spinocerebellar Ataxia','other'=>'Other');
        $dysarthia_option = array(NULL=>"", "zero"=>"0","one"=>"1", 'two'=>'2','three'=>'3');
        $spasticity_option = array(NULL=>"", "zero"=>"0","one"=>"1", "one_plus"=>"1+", 'two'=>'2','three'=>'3','four'=>'4');
        $power_option = array(NULL=>"", "zero"=>"0","one"=>"1", 'two'=>'2','three'=>'3','four_minus-'=>'4-','four'=>'4','four_plus'=>'4+','five'=>'5');
        $muscle_option = array(NULL=>"", "zero"=>"0","one"=>"1", 'two'=>'2','three'=>'3','four'=>'4');
        $other_option = array(NULL=>"", "zero"=>"0","one"=>"1", 'five'=>'5');
    	
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
        $this->form->addElement('select', 'finger_tapping_done',$this->indent . ' Has the Finger Tapping section been completed?', $tapping_option);
	$this->form->addElement('select', 'feet_tapping_done',$this->indent . ' Has the Feet Tapping section been completed?', $tapping_option);	
        $this->form->addElement('date', 'tapping_date', $this->indent . $this->indent . 'Date of Tapping:');

        $this->XINRegisterRule("f_r_1", array("finger_tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("f_l_1", array("finger_tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("f_r_2", array("finger_tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("f_l_2", array("finger_tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("fe_r_1", array("feet_tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("fe_l_1", array("feet_tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("fe_r_2", array("feet_tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("fe_l_2", array("feet_tapping_done{@}=={@}done"), "Required.");
        $this->XINRegisterRule("tapping_date", array("finger_tapping_done{@}=={@}done|feet_tapping_done{@}=={@}done"), "Required.");

            //Fingers
        $this->form->addElement('static', null, $this->indent . '<font size=3>Fingers </font><i><font size=2>(If unable, enter "0")  </font></i>.');

        $this->form->addElement('static', null, $this->indent . $this->indent . $this->indent . '<i>Trial 1</i>');
        $this->form->addElement('text', 'f_r_1', $this->indent . $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>');
        $this->form->addRule('f_r_1','Numerical value required.', 'numeric');
        $this->form->addElement('text', 'f_l_1', $this->indent . $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>');
        $this->form->addRule('f_l_1','Numerical value required.', 'numeric');

         $this->form->addElement('static', null, $this->indent . $this->indent . $this->indent . '<i>Trial 2</i>');
        $this->form->addElement('text', 'f_r_2', $this->indent . $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>');
        $this->form->addRule('f_r_2','Numerical value required.', 'numeric');
        $this->form->addElement('text', 'f_l_2', $this->indent . $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>');
        $this->form->addRule('f_l_2','Numerical value required.', 'numeric');

            //Feet
        $this->form->addElement('static', null, $this->indent . '<font size=3>Feet </font><i><font size=2>(If unable, enter "0")  </font></i>.');

        $this->form->addElement('static', null, $this->indent . $this->indent . '<i>Trial 1</i>');
        $this->form->addElement('text', 'fe_r_1', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>');
        $this->form->addRule('fe_r_1','Numerical value required.', 'numeric');
        $this->form->addElement('text', 'fe_l_1', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>');
        $this->form->addRule('fe_l_1','Numerical value required.', 'numeric');

         $this->form->addElement('static', null, $this->indent . $this->indent . '<i>Trial 2</i>');
        $this->form->addElement('text', 'fe_r_2', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>');
        $this->form->addRule('fe_r_2','Numerical value required.', 'numeric');
        $this->form->addElement('text', 'fe_l_2', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>');
        $this->form->addRule('fe_l_2','Numerical value required.', 'numeric');

        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');
        $this->form->addElement('static', null, '');

        // Speech Testing
        $this->form->addElement('static', null, '<font size=4>Speech Testing</font>');
        $this->form->addElement('select', 'speech_done',$this->indent .  '&nbsp Has Speech Testing been completed?', $done_option);
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
        $this->form->addElement('select', 'saccades_done', $this->indent . '&nbsp Has the Saccades/Antisaccades section been completed?', $done_option);
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
        $this->form->addElement('select', 'mri_done', $this->indent . '&nbsp Has the MRI been completed?', $done_option);
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

        // Neurological Exam

            // Headers 

            $this->form->addElement('static', null, '<font size=4>Neurological Exam</font>');
            $this->form->addElement('date', 'exam_date', $this->indent . '&nbspDate of Neurological Exam:');
            $this->form->addElement('text', 'physician', $this->indent . '&nbspPhysician Name:');

            // Face
            $this->form->addElement('static', null, $this->indent . '<u><font size=3>Face</font></u>', $done_option);

            $this->form->addElement('select', 'face_done', $this->indent . $this->indent . '&nbsp Has the Face section been completed?', $done_option);

                // Face Questions

                // Fasciculations
                $this->form->addElement('select', 'face_fasciculations', $this->indent . $this->indent . $this->indent . 'Fasciculations:', $yes_no_option);

                // Weakness
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Weakness:');

                    $this->form->addElement('select', 'face_weakness_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $yes_no_option);
                    $this->form->addElement('select', 'face_weakness_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $yes_no_option);

            // Face Done/Not Done rules
            $this->XINRegisterRule("face_fasciculations", array("face_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("face_weakness_left", array("face_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("face_weakness_right", array("face_done{@}=={@}done"), "Required.");


            // Breaks
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');  

            // Tongue
            $this->form->addElement('static', null, $this->indent . '<u><font size=3>Tongue</font></u>');

            $this->form->addElement('select', 'tongue_done', $this->indent . $this->indent . '&nbsp Has the Tongue section been completed?', $done_option);

                // Tongue Questions

                // Fasciculations
                $this->form->addElement('select', 'tongue_fasciculations', $this->indent . $this->indent . $this->indent . 'Fasciculations:', $yes_no_option);

                // Atrophy
                $this->form->addElement('select', 'tongue_atrophy', $this->indent . $this->indent . $this->indent . 'Atrophy:', $yes_no_option);


                // Weakness
                $this->form->addElement('select', 'tongue_weakness', $this->indent . $this->indent . $this->indent. 'Weakness:', $yes_no_option);

                   
            // Tongue Done/Not Done rules
            $this->XINRegisterRule("tongue_fasciculations", array("tongue_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("tongue_atrophy", array("tongue_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("tongue_weakness", array("tongue_done{@}=={@}done"), "Required.");
            
            // Breaks
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');  

            // Dysarthia
            $this->form->addElement('static', null, $this->indent . '<u><font size=3>Dysarthia</font></u>');

            $this->form->addElement('select', 'dysarthia_done', $this->indent . $this->indent . '&nbsp Has the Dysarthia section been completed?', $done_option);

                // Dysarthia Questions

                // Dysarthia
                $this->form->addElement('select', 'dysarthia_dysarthia', $this->indent . $this->indent . $this->indent . 'Dysarthia:', $dysarthia_option);

            // Dysarthia Done/Not Done rules
            $this->XINRegisterRule("dysarthia_dysarthia", array("dysarthia_done{@}=={@}done"), "Required.");

            // Breaks
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');  

            // Fasciculations
            $this->form->addElement('static', null, $this->indent . '<u><font size=3>Fasciculations</font></u>');

            $this->form->addElement('select', 'fasciculations_done', $this->indent . $this->indent . '&nbsp Has the Fasciculations section been completed?', $done_option);

                // Fasciculation Questions

                // Trunk
                $this->form->addElement('select', 'fasciculations_trunk', $this->indent . $this->indent . $this->indent . 'Trunk:', $yes_no_option);

                // Arm
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Arm');

                    $this->form->addElement('select', 'fasciculations_arm_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $yes_no_option);
                    $this->form->addElement('select', 'fasciculations_arm_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $yes_no_option);


               // Leg
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Leg');

                    $this->form->addElement('select', 'fasciculations_leg_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $yes_no_option);
                    $this->form->addElement('select', 'fasciculations_leg_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $yes_no_option);



            // Fasciculations Done/Not Done rules
            $this->XINRegisterRule("fasciculations_trunk", array("fasciculations_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("fasciculations_arm_right", array("fasciculations_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("fasciculations_arm_left", array("fasciculations_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("fasciculations_leg_left", array("fasciculations_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("fasciculations_leg_right", array("fasciculations_done{@}=={@}done"), "Required.");

            // Breaks
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');  

            // Atrophy
            $this->form->addElement('static', null, $this->indent . '<u><font size=3>Atrophy</font></u>');

            $this->form->addElement('select', 'atrophy_done', $this->indent . $this->indent . '&nbsp Has the Atrophy section been completed?', $done_option);

                // Atrophy Questions

                
                // Arm
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Arm');

                    $this->form->addElement('select', 'atrophy_arm_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $yes_no_option);
                    $this->form->addElement('select', 'atrophy_arm_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $yes_no_option);


               // Leg
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Leg');

                    $this->form->addElement('select', 'atrophy_leg_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $yes_no_option);
                    $this->form->addElement('select', 'atrophy_leg_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $yes_no_option);



            // Atrophy Done/Not Done rules
            $this->XINRegisterRule("atrophy_arm_right", array("atrophy_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("atrophy_arm_left", array("atrophy_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("atrophy_leg_left", array("atrophy_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("atrophy_leg_right", array("atrophy_done{@}=={@}done"), "Required.");


            // Breaks
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');  

            // Spasticity
            $this->form->addElement('static', null, $this->indent . '<u><font size=3>Spasticity</font></u>');

            $this->form->addElement('select', 'spasticity_done', $this->indent . $this->indent . '&nbsp Has the Spasticity section been completed?', $done_option);

                // Spasticity Questions

                
                // Upper Extremities
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Upper Extremities <i><font size=1>(Forearm Supination / Pronation)</font></i>');

                    $this->form->addElement('select', 'spasticity_upper_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $spasticity_option);
                    $this->form->addElement('select', 'spasticity_upper_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $spasticity_option);


               // Lower Extremities
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Lower Extremities <i><font size=1>(Knee Flexion)</font></i>');

                    $this->form->addElement('select', 'spasticity_lower_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>',$spasticity_option);
                    $this->form->addElement('select', 'spasticity_lower_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>',$spasticity_option);



            // Spasticity Done/Not Done rules
            $this->XINRegisterRule("spasticity_upper_left", array("spasticity_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("spasticity_upper_right", array("spasticity_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("spasticity_lower_left", array("spasticity_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("spasticity_lower_right", array("spasticity_done{@}=={@}done"), "Required.");


            // Breaks
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');  

            // Power
            $this->form->addElement('static', null, $this->indent . '<u><font size=3>Power</font></u>');

            $this->form->addElement('select', 'power_done', $this->indent . $this->indent .  '&nbsp Has the Power section been completed?', $done_option);

                // Power Questions

                
                // Neck extension
                $this->form->addElement('select', 'power_neck_extension', $this->indent . $this->indent . $this->indent . 'Neck extension', $power_option);

                // Neck flexion
                $this->form->addElement('select', 'power_neck_flexion', $this->indent . $this->indent . $this->indent . 'Neck flexion', $power_option);

                // Infraspinatus
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Infraspinatus');

                    $this->form->addElement('select', 'power_infraspinatus_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $power_option);
                    $this->form->addElement('select', 'power_infraspinatus_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $power_option);

                // Deltoids
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Deltoids', $power_option);

                    $this->form->addElement('select', 'power_deltoids_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $power_option);
                    $this->form->addElement('select', 'power_deltoids_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $power_option);

                // Biceps
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Biceps', $power_option);

                    $this->form->addElement('select', 'power_biceps_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $power_option);
                    $this->form->addElement('select', 'power_biceps_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $power_option);

                // Triceps
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Triceps');

                    $this->form->addElement('select', 'power_triceps_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $power_option);
                    $this->form->addElement('select', 'power_triceps_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $power_option);

                // Wrist Extensors
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Wrist Extensors');

                    $this->form->addElement('select', 'power_wrist_extensors_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $power_option);
                    $this->form->addElement('select', 'power_wrist_extensors_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $power_option);

                // Wrist Flexors
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Wrist Flexors');

                    $this->form->addElement('select', 'power_wrist_flexors_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $power_option);
                    $this->form->addElement('select', 'power_wrist_flexors_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $power_option);

                // EDC
                $this->form->addElement('static', null, $this->indent . $this->indent . ' EDC');

                    $this->form->addElement('select', 'power_edc_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $power_option);
                    $this->form->addElement('select', 'power_edc_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $power_option);

                // FDI
                $this->form->addElement('static', null, $this->indent . $this->indent . ' FDI');

                    $this->form->addElement('select', 'power_fdi_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $power_option);
                    $this->form->addElement('select', 'power_fdi_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $power_option);

                // ADM
                $this->form->addElement('static', null, $this->indent . $this->indent . ' ADM');

                    $this->form->addElement('select', 'power_adm_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $power_option);
                    $this->form->addElement('select', 'power_adm_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $power_option);

                // APB
                $this->form->addElement('static', null, $this->indent . $this->indent . ' APB');

                    $this->form->addElement('select', 'power_apb_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $power_option);
                    $this->form->addElement('select', 'power_apb_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $power_option);

                // Iliopsoas
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Iliopsoas');

                    $this->form->addElement('select', 'power_iliopsoas_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $power_option);
                    $this->form->addElement('select', 'power_iliopsoas_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $power_option);

                // Hip Abductors
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Hip Abductors');

                    $this->form->addElement('select', 'power_hip_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $power_option);
                    $this->form->addElement('select', 'power_hip_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $power_option);

                // Quadriceps
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Quadriceps');

                    $this->form->addElement('select', 'power_quadriceps_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $power_option);
                    $this->form->addElement('select', 'power_quadriceps_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $power_option);

                // Hamstrings
                $this->form->addElement('static', null, $this->indent .  $this->indent . ' Hamstrings');

                    $this->form->addElement('select', 'power_hamstrings_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $power_option);
                    $this->form->addElement('select', 'power_hamstrings_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $power_option);

                // Ankle Dorsiflexors
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Ankle Dorsiflexors');

                    $this->form->addElement('select', 'power_ankle_dorsiflexors_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $power_option);
                    $this->form->addElement('select', 'power_ankle_dorsiflexors_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $power_option);

                // Ankle Plantarflexors
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Ankle Plantarflexors');

                    $this->form->addElement('select', 'power_ankle_plantarflexors_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $power_option);
                    $this->form->addElement('select', 'power_ankle_plantarflexors_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $power_option);

                // EHL
                $this->form->addElement('static', null, $this->indent . $this->indent . ' EHL');

                    $this->form->addElement('select', 'power_ehl_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $power_option);
                    $this->form->addElement('select', 'power_ehl_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $power_option);


            // Power Done/Not Done rules
            $this->XINRegisterRule("power_neck_extension", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_neck_flexion", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_infraspinatus_left", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_infraspinatus_right", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_deltoids_left", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_deltoids_right", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_biceps_left", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_biceps_right", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_triceps_left", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_triceps_right", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_wrist_extensors_left", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_wrist_extensors_right", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_wrist_flexors_left", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_wrist_flexors_right", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_edc_left", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_edc_right", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_fdi_left", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_fdi_right", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_adm_left", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_adm_right", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_apb_left", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_apb_right", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_iliopsoas_left", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_iliopsoas_right", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_hip_left", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_hip_right", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_quadriceps_left", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_quadriceps_right", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_hamstrings_left", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_hamstrings_right", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_ankle_dorsiflexors_left", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_ankle_dorsiflexors_right", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_ankle_plantarflexors_left", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_ankle_plantarflexors_right", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_ehl_left", array("power_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("power_ehl_right", array("power_done{@}=={@}done"), "Required.");

            // Breaks
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');  

            // Muscle Stretch Reflexes
            $this->form->addElement('static', null, $this->indent . '<u><font size=3>Muscle Stretch Reflexes</font></u>');

            $this->form->addElement('select', 'muscle_done', $this->indent . $this->indent . '&nbsp Has the Muscle Stretch Reflexes section been completed?', $done_option);

                // Muscle Stretch Reflexes Questions

                
                // Jaw Jerk
                $this->form->addElement('select', 'muscle_jaw', $this->indent . $this->indent . $this->indent . 'Jaw Jerk', $muscle_option);

                // Biceps
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Biceps');

                    $this->form->addElement('select', 'muscle_biceps_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $muscle_option);
                    $this->form->addElement('select', 'muscle_biceps_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $muscle_option);

                // Brachioradialis
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Brachioradialis');

                    $this->form->addElement('select', 'muscle_brachioradialis_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $muscle_option);
                    $this->form->addElement('select', 'muscle_brachioradialis_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $muscle_option);

                // Triceps
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Triceps');

                    $this->form->addElement('select', 'muscle_triceps_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $muscle_option);
                    $this->form->addElement('select', 'muscle_triceps_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $muscle_option);

                // Quadriceps
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Quadriceps');

                    $this->form->addElement('select', 'muscle_quadriceps_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $muscle_option);
                    $this->form->addElement('select', 'muscle_quadriceps_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $muscle_option);

                // Triceps Surae
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Triceps Surae');

                    $this->form->addElement('select', 'muscle_triceps_surae_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $muscle_option);
                    $this->form->addElement('select', 'muscle_triceps_surae_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $muscle_option);



                
            // Muscle Stretch Reflexes Done/Not Done rules
            $this->XINRegisterRule("muscle_jaw", array("muscle_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("muscle_biceps_left", array("muscle_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("muscle_biceps_right", array("muscle_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("muscle_brachioradialis_left", array("muscle_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("muscle_brachioradialis_right", array("muscle_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("muscle_triceps_left", array("muscle_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("muscle_triceps_right", array("muscle_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("muscle_quadriceps_left", array("muscle_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("muscle_quadriceps_right", array("muscle_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("muscle_triceps_surae_left", array("muscle_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("muscle_triceps_surae_right", array("muscle_done{@}=={@}done"), "Required.");


            // Breaks
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');  

            // Other Reflexes
            $this->form->addElement('static', null, $this->indent . '<u><font size=3>Other Reflexes</font></u>');

            $this->form->addElement('select', 'other_done', $this->indent . $this->indent . '&nbsp Has the Other Reflexes section been completed?', $done_option);

                // Other Reflexes Questions

            
                // Hoffman's
                $this->form->addElement('static', null, $this->indent . $this->indent . " Hoffman's");

                    $this->form->addElement('select', 'other_hoffmans_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $other_option);
                    $this->form->addElement('select', 'other_hoffmans_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $other_option);

               // Superficial Abdominal 
                $this->form->addElement('static', null, $this->indent . $this->indent . " Superficial Abdominal");

                    $this->form->addElement('select', 'other_superficial_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $other_option);
                    $this->form->addElement('select', 'other_superficial_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $other_option);

                // Babinski
                $this->form->addElement('static', null, $this->indent . $this->indent . " Babinski");

                    $this->form->addElement('select', 'other_babinski_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $other_option);
                    $this->form->addElement('select', 'other_babinski_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $other_option);


                
            // Other Reflexes Done/Not Done rules
            $this->XINRegisterRule("other_hoffmans_left", array("other_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("other_hoffmans_right", array("other_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("other_superficial_left", array("other_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("other_superficial_right", array("other_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("other_babinski_left", array("other_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("other_babinski_right", array("other_done{@}=={@}done"), "Required.");

            // Breaks
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');  

            // Pseudobulbar Affect
            $this->form->addElement('static', null, $this->indent . '<u><font size=3>Pseudobulbar Affect</font></u>');

            $this->form->addElement('select', 'pseudobulbar_done', $this->indent . $this->indent . '&nbsp Has the Pseudobulbar Affect section been completed?', $done_option);

                // Pseudobulbar Affect Questions
                $this->form->addElement('select', 'pseudobulbar', $this->indent . $this->indent . $this->indent . "Pseudobulbar", $yes_no_option);

            // Pseudobulbar Affect Done/Not Done Rules
            $this->XINRegisterRule("pseudobulbar", array("pseudobulbar_done{@}=={@}done"), "Required.");

            // Breaks
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');  

            // Cerebellar Dysfunction
            $this->form->addElement('static', null, $this->indent . '<u><font size=3>Cerebellar Dysfunction</font></u>');

            $this->form->addElement('select', 'cerebellar_done', $this->indent . $this->indent . '&nbsp Has the Cerebellar Dysfunction section been completed?', $done_option);

                // Cerebellar Dysfunction Questions

                
                // Arm
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Arm (finger - nose ataxia)');

                    $this->form->addElement('select', 'cerebellar_arm_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $yes_no_unable_option);
                    $this->form->addElement('select', 'cerebellar_arm_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $yes_no_unable_option);


               // Leg
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Leg (heel - shin ataxia)');

                    $this->form->addElement('select', 'cerebellar_leg_right', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Right</i>', $yes_no_unable_option);
                    $this->form->addElement('select', 'cerebellar_leg_left', $this->indent . $this->indent . $this->indent . $this->indent . '<i>Left</i>', $yes_no_unable_option);



            // Cerebellar Dysfunction Done/Not Done rules
            $this->XINRegisterRule("cerebellar_arm_right", array("cerebellar_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("cerebellar_arm_left", array("cerebellar_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("cerebellar_arm_right", array("cerebellar_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("cerebellar_arm_left", array("cerebellar_done{@}=={@}done"), "Required.");


            // Breaks
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');  
		

	    // Gait
            $this->form->addElement('static', null, $this->indent . '<u><font size=3>Gait</font></u>');

            $this->form->addElement('select', 'gait_done', $this->indent . $this->indent . '&nbsp Has the Gait section been completed?', $done_option);

                // Gait Questions


                // Spastic

                    $this->form->addElement('select', 'gait_spastic', $this->indent . $this->indent . $this->indent .  'Spastic', $yes_no_unable_option);
                    $this->form->addElement('select', 'gait_ataxic', $this->indent . $this->indent . $this->indent . 'Ataxic', $yes_no_unable_option);


            // Gait Done/Not Done rules
            $this->XINRegisterRule("gait_spastic", array("gait_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("gait_ataxic", array("gait_done{@}=={@}done"), "Required.");

            // Breaks
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');

            // Sites of ALS Involvement
            $this->form->addElement('static', null, $this->indent . '<u><font size=3>Sites of ALS Involvement</font></u>');

            $this->form->addElement('select', 'emg_done', $this->indent . $this->indent . '&nbsp Has the Sites of ALS Involvement section been completed?', $done_option);

                // Date
                $this->form->addElement('date', 'emg_date', $this->indent . $this->indent . $this->indent . 'Date of EMG');

                // ALS Involvement Questions

                // Bulbar
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Bulbar');

                    $this->form->addElement('select', 'emg_bulbar_umn', $this->indent . $this->indent . $this->indent . $this->indent . '<i>UMN</i>', $yes_no_option);
                    $this->form->addElement('select', 'emg_bulbar_lmn', $this->indent . $this->indent . $this->indent . $this->indent . '<i>LMN</i>', $yes_no_option);
                    $this->form->addElement('select', 'emg_bulbar_denervation', $this->indent . $this->indent . $this->indent . $this->indent . '<i>EMG evidence of active denervation*</i>', $yes_no_option);

                // Cervical
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Cervical');

                    $this->form->addElement('select', 'emg_cervical_umn', $this->indent . $this->indent . $this->indent . $this->indent . '<i>UMN</i>', $yes_no_option);
                    $this->form->addElement('select', 'emg_cervical_lmn', $this->indent . $this->indent . $this->indent . $this->indent . '<i>LMN</i>', $yes_no_option);
                    $this->form->addElement('select', 'emg_cervical_denervation', $this->indent . $this->indent . $this->indent . $this->indent . '<i>EMG evidence of active denervation*</i>', $yes_no_option);

                // Thoracic
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Thoracic');

                    $this->form->addElement('select', 'emg_thoracic_umn', $this->indent . $this->indent . $this->indent . $this->indent . '<i>UMN</i>', $yes_no_option);
                    $this->form->addElement('select', 'emg_thoracic_lmn', $this->indent . $this->indent . $this->indent . $this->indent . '<i>LMN</i>', $yes_no_option);
                    $this->form->addElement('select', 'emg_thoracic_denervation', $this->indent . $this->indent . $this->indent . $this->indent . '<i>EMG evidence of active denervation*</i>', $yes_no_option);

                // Lumbar
                $this->form->addElement('static', null, $this->indent . $this->indent . ' Lumbar');

                    $this->form->addElement('select', 'emg_lumbar_umn', $this->indent . $this->indent . $this->indent . $this->indent . '<i>UMN</i>', $yes_no_option);
                    $this->form->addElement('select', 'emg_lumbar_lmn', $this->indent . $this->indent . $this->indent . $this->indent . '<i>LMN</i>', $yes_no_option);
                    $this->form->addElement('select', 'emg_lumbar_denervation', $this->indent . $this->indent . $this->indent . $this->indent . '<i>EMG evidence of active denervation*</i>', $yes_no_option);

                $this->form->addElement('static', null, $this->indent . $this->indent . '<i><font size=1> *complete at Visit 1 OR if patient has had a new EMG since last visit.</font></i>');
                    
                    


            // Sites of ALS Involvement Done/Not Done rules
            $this->XINRegisterRule("emg_date", array("emg_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("emg_bulbar_umn", array("emg_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("emg_bulbar_lmn", array("emg_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("emg_bulbar_denervation", array("emg_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("emg_cervical_umn", array("emg_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("emg_cervical_lmn", array("emg_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("emg_cervical_denervation", array("emg_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("emg_thoracic_umn", array("emg_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("emg_thoracic_lmn", array("emg_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("emg_thoracic_denervation", array("emg_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("emg_lumbar_umn", array("emg_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("emg_lumbar_lmn", array("emg_done{@}=={@}done"), "Required.");
            $this->XINRegisterRule("emg_lumbar_denervation", array("emg_done{@}=={@}done"), "Required.");


            // Breaks
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');  

            // El Escorial Criteria
            $this->form->addElement('select', 'elescorial_diagnosis', $this->indent . '<font size=3> <u>El Escorial Criteria</font></u>', $elescorial_option);

            // El Escorial Criteria  rules

            // Breaks
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');
            $this->form->addElement('static', null, '');  

            // Diagnosis
            $this->form->addElement('select', 'diagnosis', $this->indent . '<font size=3> <u>Diagnosis</font></u>', $diagnosis_option);
            $this->form->addElement('text', 'diagnosis_other', $this->indent . $this->indent . $this->indent . 'If other, specify');

            // Diagnosis rules
            $this->XINRegisterRule("diagnosis_other", array("diagnosis{@}=={@}other"), "Required.");
	    
	    // Last Notes section
     	    $this->form->addElement('textarea', 'notes_last', $this->indent . $this->indent .'<u><font size=3>Notes:</font></u>');
            $this->XINRegisterRule("notes_last",array("notes_last{@}=={@}NEVER_REQUIRED"),"Not required");

        $this->addRule('finger_tapping_done', 'Examiner is required', 'required');
	$this->addRule('feet_tapping_done', 'Examiner is required', 'required');
        $this->addRule('speech_done', 'Examiner is required', 'required');
        $this->addRule('saccades_done', 'Examiner is required', 'required');
        $this->addRule('mri_done', 'Examiner is required', 'required');
        $this->addRule('exam_date', 'Examiner is required', 'required');
        $this->addRule('physician', 'Examiner is required', 'required');
        $this->addRule('face_done', 'Examiner is required', 'required');
        $this->addRule('tongue_done', 'Examiner is required', 'required');
        $this->addRule('dysarthia_done', 'Examiner is required', 'required');
        $this->addRule('fasciculations_done', 'Examiner is required', 'required');
        $this->addRule('atrophy_done', 'Examiner is required', 'required');
        $this->addRule('spasticity_done', 'Examiner is required', 'required');
        $this->addRule('power_done', 'Examiner is required', 'required');
        $this->addRule('muscle_done', 'Examiner is required', 'required');
        $this->addRule('other_done', 'Examiner is required', 'required');
        $this->addRule('pseudobublar_done', 'Examiner is required', 'required');
        $this->addRule('cerebellar_done', 'Examiner is required', 'required');
        $this->addRule('emg_done', 'Examiner is required', 'required');
        $this->addRule('elescorial_diagnosis', 'Examiner is required', 'required');
        $this->addRule('diagnosis', 'Examiner is required', 'required');
        

        
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
                        <b>Upload Error</b> - $error. File was not uploaded, however data has been saved.
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
         
        $timepoint = TimePoint::singleton($this->getSessionID());
        $candidate = Candidate::singleton($timepoint->getCandID());
        $PSCID = $candidate->getPSCID();
        $file_name = explode("_",$args['values']['upload_file']['name']);
        $file_type = explode("_",$args['values']['upload_file']['type']);
        if ($PSCID != $file_name[0]){
            
            
            return array("Improper format: File name does not begin with PSCID.");
        }
       
     
        if (strpos($file_type[0],"audio")=== false && strpos($file_type[0],"video")=== false){
            return array("Improper format: File is not an audio type.");
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
