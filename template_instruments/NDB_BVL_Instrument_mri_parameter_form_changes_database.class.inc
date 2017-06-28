<?php
class NDB_BVL_Instrument_mri_parameter_form extends NDB_BVL_Instrument
{
    var $ValidityEnabled = true;
    var $ValidityRequired = true;


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
        $this->testName = "mri_parameter_form";           // test_names.Test_name
        $this->table = 'mri_parameter_form';             // name of table containing


        // data keyed by commentID
        $this->commentID = $commentID;

        $this->_requiredElements = array('Date_taken', 'Examiner');
        
        $this->dateTimeFields=array("Date_taken");
        
        // setup the form
        $this->_setupForm();
    }
    function getScanStatus(){
        $db =& Database::singleton();
        $scan_list = array('t1_1_0','t1_0_8','t1_other','t2_1_0','t2_0_8','t2_other','flair','r2star','dti_2_0','dti_1_5','dti_other','fMRI_3_0','fMRI_2_0','fMRI_other');
        $scan_parameters = array();
        $commentID =  $this->getCommentID();
        // auto_t1_1_0_acquisition_number_attempts
        // auto_t1_1_0_acquisition

        foreach ($scan_list as $scan) {

            $acquisition = 'auto_'.$scan.'_acquisition';
            $acquisition_count = 'auto_'.$scan.'_acquisition_number_attempts';
              
            $count_query = "SELECT $acquisition as aq, $acquisition_count as aq_c
                FROM mri_parameter_form mpf
                WHERE CommentID='$commentID'                
                ";


            $parameters = $db->pselectRow($count_query,null);
            
            $scan_parameters[$scan]['status'] = $parameters['aq'];
            $scan_parameters[$scan]['attempts'] = $parameters['aq_c'];
            
            


    
        }
        
        
        return $scan_parameters;
    }
    function generateHTML($scan_parameters){
        $html = "<div class='table-responsive'>
                                            <table class='table table-bordered'>
                                                <thead>
                                                    <tr class='info'>
                                                        <th nowrap='nowrap' colspan='3'>
                                                            Scan
                                                        </th>
                                                        <th nowrap='nowrap' colspan='3'>
                                                            Status
                                                        </th>
                                                        <th nowrap='nowrap' colspan='3'>
                                                            Attempts
                                                        </th>
                                                        
                                                    </tr>
                                                </thead>
                                                <tbody>";
        $wrapper = "<td nowrap='nowrap' colspan='3'>";
        foreach ($scan_parameters as $scan => $value) {

            if ($scan=='t1_1_0') {
                $scan = 'T1 1.0';
            } elseif ($scan=='t1_0_8') {
                $scan = 'T1 0.8';
            } elseif ($scan=='t1_other') {
                $scan = 'Other T1';
            } elseif ($scan=='t2_1_0') {
                $scan ='T2 1.0';
            } elseif ($scan=='t2_0_8') {
               $scan = 'T2 0.8';
            } elseif ($scan=='t2_other') {
                $scan = 'Other T2';
            } elseif ($scan=='dti_2_0') {
                $scan = 'DTI 2.0';
            } elseif ($scan=='dti_1_5') {
                $scan = 'DTI 1.5';
            } elseif ($scan=='dti_other') {
                $scan = 'Other DTI';
            } elseif ($scan=='fMRI_3_0') {
                $scan = 'fMRI 3.0';
            } elseif ($scan=='fMRI_2_0') {
                $scan = 'fMRI 2.0';
            } elseif ($scan=='fMRI_other') {
                $scan = 'Other fMRI';
            } elseif ($scan=='flair') {
                $scan = 'Flair';

            } elseif ($scan=='r2star') {
                $scan = 'R2star';
                
            }
            if ($value[status]=='Yes') {
                $value[status] = "<font color=' #2E8B57'>".$value[status]."</font>";
            } elseif ($value[status]=='None') {
                $value[status] = "<font color='#b20000'>".$value[status]."</font>";
            }
            $html_scan = "<tr>".$wrapper.$scan."</td>".$wrapper.$value[status]."</td>".$wrapper.$value[attempts]."</td></tr>";

            //$html_scan .= $wrapper.$scan."</td>";
            //$html_scan .= $wrapper.$value[status]."</td>";
            //$html_scan .= $wrapper.$value[attempts]."</td></tr>";
            $html .= $html_scan;         
            
        }
        $html .= "</tbody></table></div>";
        return $html;
        
    }  


    /**
    * method to build the HTML_Quickform object into a paged form
    *
    * @return void
    * @access private
    */
    function _setupForm(){

        $this->score();
        $scan_parameters = $this->getScanStatus();
        $html = $this->generateHTML($scan_parameters); 
        print_r($html);
        $this->form->addElement('static', null, $html);

        //Defines the call back function for HTML Quickform to use when validating the form.
        $this->form->addFormRule(array(&$this,'XINValidate'));

        
        // Automated Header
        $this->form->addElement('header', null, 'Automated Summary of Scan');

        // t1_1_0 Automated
        $this->form->addElement('static', 'auto_t1_1_0_acquisition', "Was the <font color='#1c70b6'>T1 1.0</font> sequence acquired?");
        $this->form->addElement('static', "auto_t1_1_0_acquisition_number_attempts", $this->indent . "Number of scan(s):");

        // t1_0_8 Automated
        $this->form->addElement('static', 'auto_t1_0_8_acquisition', "Was the <font color='#1c70b6'>T1 0.8</font> sequence acquired?");
        $this->form->addElement('static', "auto_t1_0_8_acquisition_number_attempts", $this->indent . "Number of scan(s):");

        // t1_other Automated
        $this->form->addElement('static', 'auto_t1_other_acquisition', "Were other <font color='#1c70b6'>T1</font> sequence(s) acquired?");
        $this->form->addElement('static', "auto_t1_other_acquisition_number_attempts", $this->indent . "Number of scan(s):");

        // t2_1_0 Automated
        $this->form->addElement('static', 'auto_t2_1_0_acquisition', "Was the <font color='#1c70b6'>T2 1.0</font> sequence acquired?");
        $this->form->addElement('static', "auto_t2_1_0_acquisition_number_attempts", $this->indent . "Number of scan(s):");

        // t2_0_8 Automated
        $this->form->addElement('static', 'auto_t2_0_8_acquisition', "Was the <font color='#1c70b6'>T1 0.8</font> sequence acquired?");
        $this->form->addElement('static', "auto_t2_0_8_acquisition_number_attempts", $this->indent . "Number of scan(s):");

        // t2_other Automated
        $this->form->addElement('static', 'auto_t2_other_acquisition', "Were other <font color='#1c70b6'>T2</font> sequence(s) acquired?");
        $this->form->addElement('static', "auto_t2_other_acquisition_number_attempts", $this->indent . "Number of scan(s):");

        // flair Automated
        $this->form->addElement('static', 'auto_flair_acquisition', "Was the <font color='#1c70b6'>FLAIR</font> sequence acquired?");
        $this->form->addElement('static', "auto_flair_acquisition_number_attempts", $this->indent . "Number of scan(s):");

        // r2star Automated
        $this->form->addElement('static', 'auto_r2star_acquisition', "Was the <font color='#1c70b6'>R2Star</font> sequence acquired?");
        $this->form->addElement('static', "auto_r2star_acquisition_number_attempts", $this->indent . "Number of scan(s):");

        // dti_2_0 Automated
        $this->form->addElement('static', 'auto_dti_2_0_acquisition', "Was the <font color='#1c70b6'>DTI 2.0</font> sequence acquired?");
        $this->form->addElement('static', "auto_dti_2_0_acquisition_number_attempts", $this->indent . "Number of scan(s):");

        // dti_1_5 Automated
        $this->form->addElement('static', 'auto_dti_1_5_acquisition', "Was the <font color='#1c70b6'>DTI 1.5</font> sequence acquired?");
        $this->form->addElement('static', "auto_dti_1_5_acquisition_number_attempts", $this->indent . "Number of scan(s):");

        // dti_other Automated 
        $this->form->addElement('static', 'auto_dti_other_acquisition', "Were other <font color='#1c70b6'>DTI</font> sequence(s) acquired?");
        $this->form->addElement('static', "auto_dti_other_acquisition_number_attempts", $this->indent . "Number of scan(s):");

        // fMRI_3_0 Automated
        $this->form->addElement('static', 'auto_fMRI_3_0_acquisition', "Was the <font color='#1c70b6'>fMRI 3.0</font> sequence acquired?");
        $this->form->addElement('static', "auto_fMRI_3_0_acquisition_number_attempts", $this->indent . "Number of scan(s):");

        // fMRI_2_0 Automated
        $this->form->addElement('static', 'auto_fMRI_2_0_acquisition', "Was the <font color='#1c70b6'>fMRI 2.0</font> sequence acquired?");
        $this->form->addElement('static', "auto_fMRI_2_0_acquisition_number_attempts", $this->indent . "Number of scan(s):");

        // fMRI_other Automated
        $this->form->addElement('static', 'auto_fMRI_other_acquisition', "Were other <font color='#1c70b6'>fMRI</font> sequence(s) acquired?");
        $this->form->addElement('static', "auto_fMRI_other_acquisition_number_attempts", $this->indent . "Number of scan(s):");
        $this->score();

        // extra Automated
        $this->form->addElement('static', 'auto_extra_acquisition', "Were other <font color='#1c70b6'>Extra</font> sequence(s) acquired?");
        $this->form->addElement('static', "auto_extra_acquisition_number_attempts", $this->indent . "Number of scan(s):");
        

        // display test name
        $this->form->addElement('header', 'instrument_title', "MRI Parameter Form");

        // automatically adds examiner & date of administration
        $this->_addMetadataFields();
        
        $this->form->addElement("select", "Scanner_Type", "Type of Scanner:", array("" => NULL, "siemens_64ch" => "Siemens Prisma 64ch", "seimens_20ch" => "Siemens Prisma 20ch","ge_32ch"=>"GE MR750 32ch","ge_8ch"=>"GE MR750 8ch", "philips_8ch"=>"Philips Achieva Tx 8ch", "other" => "Other"));
        $this->form->addElement("text", "Scanner_Type_other", "If other, specify:");
        $this->XINRegisterRule("Scanner_Type_other", array("Scanner_Type{@}=={@}other"), "Required if scanner type is other");

        $this->form->addElement('select','sedation', "Was sedation administered?", array(""=>NULL,"Yes" => "Yes", "No"=>"No"));
        $this->form->addElement('select','medication', "Was the subject on any medication?", array(""=>NULL,"Yes" => "Yes", "No"=>"No", 'not_answered'=>'Not Answered'));

        $this->form->addElement("text", "medication_specify", "If yes, specify:");
        $this->XINRegisterRule("medication_specify", array("medication{@}=={@}Yes"), "Required if subject was on medication!");
        $yes_no_option= array(""=>NULL, "No"=>"No", "Complete"=>"Yes, Complete Acquisition", "Partial"=>"Yes, Partial Acquisition", "not_answered"=>"Not Answered");

        

        $this->form->addElement('header', null, '3D T1 MP-Rage');
        $this->form->addElement('select', 't1_1_0_Scan_done', 'Was the T1 1.0 sequence acquired?', $yes_no_option);
        $this->addTextAreaElement('t1_1_0_Comments', 'Provide any comments regarding the T1 1.0 acquisition');
        $this->form->addElement('select', 't1_0_8_Scan_done', 'Was the T1 0.8 sequence acquired?', $yes_no_option);
        $this->addTextAreaElement('t1_0_8_Comments', 'Any comments regarding the T1 0.8 acquisition');
        $this->form->addElement('select', 't1_other_Scan_done', 'Were other T1 scans done?', $yes_no_option);
        $this->form->addElement('textarea','t1_other_Comments', 'Please provide the T1 resolutions that were acquired');
        $this->XINRegisterRule("t1_other_Comments", array("t1_other_Scan_done{@}=={@}Complete|Partial"), "Required if other T1 acquisition were acquired!");

        $this->form->addElement('header', null, 'High Resolution 3D T2');
        $this->form->addElement('select', 't2_1_0_Scan_done', 'Was the T2 1.0 sequence acquired?', $yes_no_option);
        $this->addTextAreaElement('t2_1_0_Comments', 'Provide any comments regarding the T2 1.0 acquisition');
        $this->form->addElement('select', 't2_0_8_Scan_done', 'Was the T2 0.8 sequence acquired?', $yes_no_option);
        $this->addTextAreaElement('t2_0_8_Comments', 'Any comments regarding the T2 0.8 acquisition');
        $this->form->addElement('select', 't2_other_Scan_done', 'Were other T2 scans done?', $yes_no_option);
        $this->form->addElement('textarea','t2_other_Comments', 'Please provide the T2 resolutions that were acquired');
        $this->XINRegisterRule("t2_other_Comments", array("t2_other_Scan_done{@}=={@}Complete|Partial"), "Required if other T2 acquisition were acquired!");

        $this->form->addElement('header', null, 'T2 FLAIR');
        $this->form->addElement('select', 'flair_Scan_done', 'Was the FLAIR sequence acquired?', $yes_no_option);
        $this->addTextAreaElement('flair_Comments', 'Provide any comments regarding the FLAIR acquisition');

        $this->form->addElement('header', null, '3D QSM');
        $this->form->addElement('select', 'r2star_Scan_done', 'Was the R2STAR sequence acquired?', $yes_no_option);
        $this->addTextAreaElement('r2star_Comments', 'Provide any comments regarding the R2STAR acquisition');

        
        $this->form->addElement('header', null, '60 Direction DTI');
        $this->form->addElement('select', 'dti_2_0_Scan_done', 'Was the DTI 2.0 sequence acquired?', $yes_no_option);
        $this->addTextAreaElement('dti_2_0_Comments', 'Provide any comments regarding the DTI 2.0 acquisition');
        $this->form->addElement('select', 'dti_1_5_Scan_done', 'Was the DTI 1.5 sequence acquired?', $yes_no_option);
        $this->addTextAreaElement('dti_1_5_Comments', 'Any comments regarding the DTI 1.5 acquisition');
        $this->form->addElement('select', 'dti_other_Scan_done', 'Were other DTI scans done?', $yes_no_option);
        $this->form->addElement('textarea','dti_other_Comments', 'Please provide the DTI resolutions that were acquired');
        $this->XINRegisterRule("dti_other_Comments", array("dti_other_Scan_done{@}=={@}Complete|Partial"), "Required if other DTI acquisition were acquired!");

        $this->form->addElement('header', null, 'Resting BOLD');
        $this->form->addElement('select', 'fMRI_3_0_Scan_done', 'Was the fMRI 3.0 sequence acquired?', $yes_no_option);
        $this->form->addElement('text', 'fMRI_3_0_Number_Complete_Runs', 'If partial acquiistion, then provide the number of complete runs:');
        $this->XINRegisterRule("fMRI_3_0_Number_Complete_Runs", array("fMRI_3_0_Scan_done{@}=={@}Partial"), "Required if fMRI 3.0 acquisition was partial!");
        $this->addTextAreaElement('fMRI_3_0_Comments', 'Provide any comments regarding the fMRI 3.0 acquisition');
        $this->form->addElement('select', 'fMRI_2_0_Scan_done', 'Was the fMRI 2.0 sequence acquired?', $yes_no_option);
        $this->form->addElement('text', 'fMRI_2_0_Number_Complete_Runs', 'If partial acquiistion, then provide the number of complete runs:');
        $this->XINRegisterRule("fMRI_2_0_Number_Complete_Runs", array("fMRI_2_0_Scan_done{@}=={@}Partial"), "Required if fMRI 2.0 acquisition was partial!");
        $this->addTextAreaElement('fMRI_3_0_Comments', 'Provide any comments regarding the fMRI 2.0 acquisition');
        $this->form->addElement('select', 'fMRI_other_Scan_done', 'Were other fMRI scans done?', $yes_no_option);
        $this->form->addElement('textarea','fMRI_other_Comments', 'Please provide the fMRI resolutions that were acquired');
        $this->XINRegisterRule("fMRI_other_Comments", array("fMRI_other_Scan_done{@}=={@}Complete|Partial"), "Required if other fMRI acquisition were acquired!");
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

    function score() {
    

    $db =& Database::singleton();
    //Get raw questions point values
    $query = "SELECT * FROM " . $this->table .
        " WHERE CommentID='" . $this->getCommentID() . "'";
    $record = $db->pselectRow($query, null);
    $scores = array();
    $candid = substr($this->getCommentID(),0,6);
    $pscid =  substr($this->getCommentID(),6,6);
    $count_query = "SELECT COUNT(*)
                FROM files fi
                    JOIN session s ON (fi.SessionID=s.ID)
                    JOIN candidate c ON (c.CandID=s.CandID)
                WHERE s.Current_stage <> 'Recycling Bin'
                    AND s.Active='Y'
                    AND c.Active='Y'
                    AND s.CenterID <> '1'
                    AND fi.FileType='mnc'
                    AND fi.File LIKE CONCAT('%_', :scan, '_%')
                    AND c.CandID = :CandID
                    AND c.PSCID = :PSCID
                    
                ";

    // t1_1_0 Automated
    $scores['auto_t1_1_0_acquisition_number_attempts'] = $db->pselectOne($count_query, array('CandID'=>$candid,'PSCID'=>$pscid,'scan'=>'t1_1_0'));

    if($scores['auto_t1_1_0_acquisition_number_attempts']>0) {
        $scores['auto_t1_1_0_acquisition'] = 'Yes';
    } else {
   
        $scores['auto_t1_1_0_acquisition'] = 'None';
    
    }
    // t1_0_8 Automated
    $scores['auto_t1_0_8_acquisition_number_attempts'] = $db->pselectOne($count_query, array('CandID'=>$candid,'PSCID'=>$pscid,'scan'=>'t1_0_8'));

    if($scores['auto_t1_0_8_acquisition_number_attempts']>0) {
        $scores['auto_t1_0_8_acquisition'] = 'Yes';
    } else {
   
        $scores['auto_t1_0_8_acquisition'] = 'None';
    
    }
    // t1_other Automated
    $scores['auto_t1_other_acquisition_number_attempts'] = $db->pselectOne($count_query, array('CandID'=>$candid,'PSCID'=>$pscid,'scan'=>'t1_other'));

    if($scores['auto_t1_other_acquisition_number_attempts']>0) {
        $scores['auto_t1_other_acquisition'] = 'Yes';
    } else {
   
        $scores['auto_t1_other_acquisition'] = 'None';
    
    }
    // t2_1_0 Automated
    $scores['auto_t2_1_0_acquisition_number_attempts'] = $db->pselectOne($count_query, array('CandID'=>$candid,'PSCID'=>$pscid,'scan'=>'t2_1_0'));

    if($scores['auto_t2_1_0_acquisition_number_attempts']>0) {
        $scores['auto_t2_1_0_acquisition'] = 'Yes';
    } else {
   
        $scores['auto_t2_1_0_acquisition'] = 'None';
    
    }
    // t2_0_8 Automated
    $scores['auto_t2_0_8_acquisition_number_attempts'] = $db->pselectOne($count_query, array('CandID'=>$candid,'PSCID'=>$pscid,'scan'=>'t2_0_8'));

    if($scores['auto_t2_0_8_acquisition_number_attempts']>0) {
        $scores['auto_t2_0_8_acquisition'] = 'Yes';
    } else {
   
        $scores['auto_t2_0_8_acquisition'] = 'None';
    
    }
    // t2_other Automated
    $scores['auto_t2_other_acquisition_number_attempts'] = $db->pselectOne($count_query, array('CandID'=>$candid,'PSCID'=>$pscid,'scan'=>'t2_other'));

    if($scores['auto_t2_other_acquisition_number_attempts']>0) {
        $scores['auto_t2_other_acquisition'] = 'Yes';
    } else {
   
        $scores['auto_t2_other_acquisition'] = 'None';
    
    }
    // flair Automated
    $scores['auto_flair_acquisition_number_attempts'] = $db->pselectOne($count_query, array('CandID'=>$candid,'PSCID'=>$pscid,'scan'=>'flair'));

    if($scores['auto_flair_acquisition_number_attempts']>0) {
        $scores['auto_flair_acquisition'] = 'Yes';
    } else {
   
        $scores['auto_flair_acquisition'] = 'None';
    
    }
    // r2star Automated
    $scores['auto_r2star_acquisition_number_attempts'] = $db->pselectOne($count_query, array('CandID'=>$candid,'PSCID'=>$pscid,'scan'=>'r2star'));

    if($scores['auto_r2star_acquisition_number_attempts']>0) {
        $scores['auto_r2star_acquisition'] = 'Yes';
    } else {
   
        $scores['auto_r2star_acquisition'] = 'None';
    
    }
    
    // dti_2_0 Automated
    $scores['auto_dti_2_0_acquisition_number_attempts'] = $db->pselectOne($count_query, array('CandID'=>$candid,'PSCID'=>$pscid,'scan'=>'dti_2_0'));

    if($scores['auto_dti_2_0_acquisition_number_attempts']>0) {
        $scores['auto_dti_2_0_acquisition'] = 'Yes';
    } else {
   
        $scores['auto_dti_2_0_acquisition'] = 'None';
    
    }
    // dti_1_5 Automated
    $scores['auto_dti_1_5_acquisition_number_attempts'] = $db->pselectOne($count_query, array('CandID'=>$candid,'PSCID'=>$pscid,'scan'=>'dti_1_5'));

    if($scores['auto_dti_1_5_acquisition_number_attempts']>0) {
        $scores['auto_dti_1_5_acquisition'] = 'Yes';
    } else {
   
        $scores['auto_dti_1_5_acquisition'] = 'None';
    
    }
    // dti_other Automated
    $scores['auto_dti_other_acquisition_number_attempts'] = $db->pselectOne($count_query, array('CandID'=>$candid,'PSCID'=>$pscid,'scan'=>'dti_other'));

    if($scores['auto_dti_other_acquisition_number_attempts']>0) {
        $scores['auto_dti_other_acquisition'] = 'Yes';
    } else {
   
        $scores['auto_dti_other_acquisition'] = 'None';
    
    } 
    // fMRI_3_0 Automated
     $scores['auto_fMRI_3_0_acquisition_number_attempts'] = $db->pselectOne($count_query, array('CandID'=>$candid,'PSCID'=>$pscid,'scan'=>'fMRI_3_0'));

    if($scores['auto_fMRI_3_0_acquisition_number_attempts']>0) {
        $scores['auto_fMRI_3_0_acquisition'] = 'Yes';
    } else {
   
        $scores['auto_fMRI_3_0_acquisition'] = 'None';
    
    }
    // fMRI_2_0 Automated
     $scores['auto_fMRI_2_0_acquisition_number_attempts'] = $db->pselectOne($count_query, array('CandID'=>$candid,'PSCID'=>$pscid,'scan'=>'fMRI_2_0'));

    if($scores['auto_fMRI_2_0_acquisition_number_attempts']>0) {
        $scores['auto_fMRI_2_0_acquisition'] = 'Yes';
    } else {
   
        $scores['auto_fMRI_2_0_acquisition'] = 'None';
    
    }
    // fMRI_other Automated
     $scores['auto_fMRI_other_acquisition_number_attempts'] = $db->pselectOne($count_query, array('CandID'=>$candid,'PSCID'=>$pscid,'scan'=>'fMRI_other'));

    if($scores['auto_fMRI_other_acquisition_number_attempts']>0) {
        $scores['auto_fMRI_other_acquisition'] = 'Yes';
    } else {
   
        $scores['auto_fMRI_other_acquisition'] = 'None';
    
    }

    // extra Automated
    $scores['auto_fMRI_other_acquisition_number_attempts'] = $db->pselectOne($count_query, array('CandID'=>$candid,'PSCID'=>$pscid,'scan'=>'fMRI_other'));

    if($scores['auto_fMRI_other_acquisition_number_attempts']>0) {
        $scores['auto_fMRI_other_acquisition'] = 'Yes';
    } else {
   
        $scores['auto_fMRI_other_acquisition'] = 'None';
    
    }
    // save the scores
    $db->update($this->table, $scores, array('CommentID'=>$this->getCommentID()));

    }
    
}
?>
