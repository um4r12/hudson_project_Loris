CREATE TABLE `ssf` (
`CommentID` varchar(255) NOT NULL default '',
                          `UserID` varchar(255) default NULL,
                          `Examiner` varchar(255) default NULL,
                          `Testdate` timestamp DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
                          `Data_entry_completion_status` enum('Incomplete','Complete') NOT NULL default 'Incomplete',
`Date_taken` date default NULL,
`Candidate_Age` varchar(255) default NULL,
`Window_Difference` varchar(255) default NULL,
`form_type` enum('initial','update','no_change','not_answered') default NULL,
`active` enum('yes','no','not_answered') default NULL,
`consent_date` date default NULL,
`withdrawn` enum('yes','no','not_answered') default NULL,
`withdrawn_date` date default NULL,
`lost_to_followup` enum('yes','no','not_answered') default NULL,
`lost_to_followup_date` date default NULL,
`deceased` enum('yes','no','not_answered') default NULL,
`deceased_date` date default NULL,
`is_patient` enum('yes','no','not_answered') default NULL,
`peg_insertion` enum('yes','no','not_answered') default NULL,
`peg_insertion_date` date default NULL,
`bipap_start` enum('yes','no','not_answered') default NULL,
`bipap_start_date` date default NULL,
`pav` enum('yes','no','not_answered') default NULL,
`pav_date` date default NULL,
`tracheostomy` enum('yes','no','not_answered') default NULL,
`tracheostomy_date` date default NULL,
`notes` text default NULL,
`notes_status` enum('not_answered') default NULL,
PRIMARY KEY  (`CommentID`)
);
