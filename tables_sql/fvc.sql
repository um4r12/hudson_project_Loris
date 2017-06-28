CREATE TABLE `fvc` (
`CommentID` varchar(255) NOT NULL default '',

                            `UserID` varchar(255) default NULL,

                            `Examiner` varchar(255) default NULL,

                            `Testdate` timestamp DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,

                            `Data_entry_completion_status` enum('Incomplete','Complete') NOT NULL default 'Incomplete',
`Date_taken` date default NULL,
`Candidate_Age` varchar(255) default NULL,
`Window_Difference` varchar(255) default NULL,
`test_type` enum('Forced Vital Capacity','Slow Vital Capacity','Not Done') default NULL,
`trial_one` varchar(255) default NULL,
`trial_two` varchar(255) default NULL,
`trial_three` varchar(255) default NULL,
`trial_four` varchar(255) default NULL,
`trial_five` varchar(255) default NULL,
`best_trial_volume` varchar(255) default NULL,
`best_trial_percentage` varchar(255) default NULL,
`ethnicity` enum('Caucasian','Asian','Aboriginal','Hispanic','Black or African-Canadian','Other') default NULL,
`other_ethnicity` varchar(255) default NULL,
`predicted_reference` enum('Knudsen 83','CRAPO','NHANES III','Canadian Ref 04','ERS/ECCS','Other') default NULL,
`other_predicted_reference` varchar(255) default NULL,
`height` varchar(255) default NULL,
`height_scale` enum('cm','inches') default NULL,
`weight` varchar(255) default NULL,
`weight_scale` enum('kg','lbs') default NULL,
`breathing_conditions` varchar(255) default NULL,
`other_breathing_conditions` varchar(255) default NULL,
`smoker` enum('Yes','No','Former') default NULL,
`fvc_reliability` enum('Reliable','Unreliable','Not Available','Unknown') default NULL,
`notes` text default NULL,
PRIMARY KEY  (`CommentID`)

              );
