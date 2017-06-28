<?php
class NDB_BVL_Instrument_mri_parameter_form extends NDB_BVL_Instrument
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
        $this->testName = "mri_parameter_form";           // test_names.Test_name
        $this->table = 'mri_parameter_form';             // name of table containing


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
    function getScanStatus(){
        $db =& Database::singleton();
        // List of the scans that need to be displayed as part of the automatic quality check. Can also be automated, instead of manually inserting the scan names.
        // This can be achieved by querying the database and joining the mri_scan_type table with the mri_protocol table.
        // Further exclusion criteria can be specified in the 'WHERE' part of the statement.
        $scan_list = array('t1_1.0','t1_0.8','t1_other','t2_1.0','t2_0.8','t2_other','flair','r2star','dti_2.0','dti_1.5','dti_other','fMRI_3.0','fMRI_2.0','fMRI_other','extra');
        // All the scan names, whether or not they were conducted, and the number of attempts are stored here.
        $scan_parameters = array();
        // This retreives the url variables (test_name, candID, sessionID, commentID - if any)
        $query_string = $_SERVER['QUERY_STRING'];
        parse_str($query_string,$url_parameters);
        $candid = $url_parameters['candID'];
        $sessionID = $url_parameters['sessionID'];
        $pscid_query = "SELECT PSCID from candidate where candID= :CandID";
        // This retreives the PSCID from the database using the candid that was obtained from the url parameters
        $pscid =  $db->pselectOne($pscid_query, array('CandID'=>$candid));
        // The query counts the occurences of a specific scan for a specific subject on a specific visit.
        $count_query = "SELECT COUNT(*)
                FROM files fi
                    JOIN session s ON (fi.SessionID=s.ID)
                    JOIN candidate c ON (c.CandID=s.CandID)
                WHERE s.Current_stage <> 'Recycling Bin'
                    AND fi.FileType='mnc'
                    AND fi.File LIKE CONCAT('%_', :scan, '_%')
                    AND c.CandID = :CandID
                    AND c.PSCID = :PSCID
                    AND s.ID = :sessionID
                    
                ";

        foreach ($scan_list as $scan) {
            
            $scan_parameters[$scan]['attempts'] = $db->pselectOne($count_query, array('CandID'=>$candid,'PSCID'=>$pscid,'scan'=>$scan,'sessionID'=>$sessionID));
            if ($scan_parameters[$scan]['attempts']>0) {
                $scan_parameters[$scan]['status'] = 'Yes';
            } else {
       
                $scan_parameters[$scan]['status'] = 'None';
            
            }    
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
            // Cleans up the name of the scan and makes it more presentable.
            if ($scan=='t1_1.0') {
                $scan = 'T1 1.0';
            } elseif ($scan=='t1_0.8') {
                $scan = 'T1 0.8';
            } elseif ($scan=='t1_other') {
                $scan = 'Other T1';
            } elseif ($scan=='t2_1.0') {
                $scan ='T2 1.0';
            } elseif ($scan=='t2_0.8') {
               $scan = 'T2 0.8';
            } elseif ($scan=='t2_other') {
                $scan = 'Other T2';
            } elseif ($scan=='dti_2.0') {
                $scan = 'DTI 2.0';
            } elseif ($scan=='dti_1.5') {
                $scan = 'DTI 1.5';
            } elseif ($scan=='dti_other') {
                $scan = 'Other DTI';
            } elseif ($scan=='fMRI_3.0') {
                $scan = 'fMRI 3.0';
            } elseif ($scan=='fMRI_2.0') {
                $scan = 'fMRI 2.0';
            } elseif ($scan=='fMRI_other') {
                $scan = 'Other fMRI';
            } elseif ($scan=='flair') {
                $scan = 'Flair';

            } elseif ($scan=='r2star') {
                $scan = 'R2star';
                
            } elseif ($scan=='extra') {
                $scan = 'Extra (Unknown)';
                
            }
            if ($value['status']=='Yes') {
                // color is green
                $value['status'] = "<font color=' #2E8B57'>".$value['status']."</font>";
            } elseif ($value['status']=='None') {
                // color is red
                $value['status'] = "<font color='#b20000'>".$value['status']."</font>";
            }
           
            // The following three lines are pooled in the line that follows.
            //$html_scan .= $wrapper.$scan."</td>";
            //$html_scan .= $wrapper.$value[status]."</td>";
            //$html_scan .= $wrapper.$value[attempts]."</td></tr>";

            $html_scan = "<tr>".$wrapper.$scan."</td>".$wrapper.$value['status']."</td>".$wrapper.$value['attempts']."</td></tr>";
            $html .= $html_scan;         
            
        }
        $html .= "<td bgcolor='#1c70b6' nowrap='nowrap' colspan='7'><font color='#ffffff'><em>Summary: </em></font></td></tbody></table></div>";
        return $html;
        
    }  


    /**
    * method to build the HTML_Quickform object into a paged form
    *
    * @return void
    * @access private
    */
    function _setupForm(){

        $scan_parameters = $this->getScanStatus();
        $html = $this->generateHTML($scan_parameters); 
        $this->form->addElement('static', null, $html);

        //Defines the call back function for HTML Quickform to use when validating the form.
        $this->form->addFormRule(array(&$this,'XINValidate'));


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
        $this->form->addElement('text', 'fMRI_3_0_Number_Complete_Runs', 'If partial acquisition, then provide the number of complete runs:');
        $this->XINRegisterRule("fMRI_3_0_Number_Complete_Runs", array("fMRI_3_0_Scan_done{@}=={@}Partial"), "Required if fMRI 3.0 acquisition was partial!");
        $this->addTextAreaElement('fMRI_3_0_Comments', 'Provide any comments regarding the fMRI 3.0 acquisition');
        $this->form->addElement('select', 'fMRI_2_0_Scan_done', 'Was the fMRI 2.0 sequence acquired?', $yes_no_option);
        $this->form->addElement('text', 'fMRI_2_0_Number_Complete_Runs', 'If partial acquisition, then provide the number of complete runs:');
        $this->XINRegisterRule("fMRI_2_0_Number_Complete_Runs", array("fMRI_2_0_Scan_done{@}=={@}Partial"), "Required if fMRI 2.0 acquisition was partial!");
        $this->addTextAreaElement('fMRI_2_0_Comments', 'Provide any comments regarding the fMRI 2.0 acquisition');
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
        // Commented this out as this will extract the examiners for the clinical data
        //$examiners = $this->_getExaminerNames();
        // Add more options when the information is available.
        $examiners = array("" => NULL, "peter_seres" => "Peter Seres"); 
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
