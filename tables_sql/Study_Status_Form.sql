CREATE TABLE `Study_Status_Form` (
`CommentID` varchar(255) NOT NULL default '',

                            `UserID` varchar(255) default NULL,

                            `Examiner` varchar(255) default NULL,

                            `Testdate` timestamp DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,

                            `Data_entry_completion_status` enum('Incomplete','Complete') NOT NULL default 'Incomplete',
`Date_taken` date default NULL,
`form_type` enum('Initial','Update','No Change') default NULL,
`status_participant` enum('Active','Withdrawn','Lost-to Follow-up','Deceased') default NULL,
`status_date` date default NULL,
`peg_insertion` enum('Yes','No') default NULL,
`peg_insertion_date` date default NULL,
`bipap_start` enum('Yes','No') default NULL,
`bipap_start_date` date default NULL,
`pav` enum('Yes','No') default NULL,
`pav_date` date default NULL,
`tracheostomy` enum('Yes','No') default NULL,
`tracheostomy_date` date default NULL,
`notes` text default NULL,
PRIMARY KEY  (`CommentID`)

              );
