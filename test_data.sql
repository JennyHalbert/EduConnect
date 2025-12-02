----------------------------------------------------------------------
-- STUDENTS
----------------------------------------------------------------------
INSERT INTO students (name, email, password) VALUES
('Alice Johnson',   'alice@tru.ca',   'pass1'),
('Brian Smith',     'brian@tru.ca',   'pass2'),
('Cathy Nguyen',    'cathy@tru.ca',   'pass3'),
('David Martin',    'david@tru.ca',   'pass4'),
('Emily Watson',    'emily@tru.ca',   'pass5'),
('Franklin Cooper', 'frank@tru.ca',   'pass6');

----------------------------------------------------------------------
-- TUTORS
-- days format: Mon–Sun → 7 bits (e.g., "1010100")
----------------------------------------------------------------------
INSERT INTO tutors (name, email, password, days, total_ratings, total_completed, total_matched) VALUES
('Sarah Thompson', 'sarah@edu.ca', 'tutor1', '1010100', 25.0, 10, 12),
('Jacob Reynolds', 'jacob@edu.ca', 'tutor2', '0110010', 40.0, 20, 25),
('Olivia Perez',   'olivia@edu.ca', 'tutor3','1111100', 18.0, 7, 10),
('Michael Lee',    'mike@edu.ca',   'tutor4','0000111', 32.0, 15, 17),
('Sophie Kim',     'sophie@edu.ca', 'tutor5','0101000', 10.0, 3, 4);

----------------------------------------------------------------------
-- SUBJECTS
-- Linked via tutor_email (FOREIGN KEY)
----------------------------------------------------------------------
-- Sarah
INSERT INTO subjects (tutor_email, subject) VALUES
('sarah@edu.ca', 'Math'),
('sarah@edu.ca', 'Physics');

-- Jacob
INSERT INTO subjects (tutor_email, subject) VALUES
('jacob@edu.ca', 'Biology'),
('jacob@edu.ca', 'Chemistry'),
('jacob@edu.ca', 'Math');

-- Olivia
INSERT INTO subjects (tutor_email, subject) VALUES
('olivia@edu.ca', 'English'),
('olivia@edu.ca', 'Writing'),
('olivia@edu.ca', 'History');

-- Michael
INSERT INTO subjects (tutor_email, subject) VALUES
('mike@edu.ca', 'Computer Science'),
('mike@edu.ca', 'Math');

-- Sophie
INSERT INTO subjects (tutor_email, subject) VALUES
('sophie@edu.ca', 'Economics'),
('sophie@edu.ca', 'Finance'),
('sophie@edu.ca', 'Statistics');

----------------------------------------------------------------------
-- REQUESTS
-- MATCHES EXACTLY:
-- id (autoinc), student_email, tutor_email, subject, description,
-- urgency, status, has_rated, is_accepted, days
--
-- STATUS CODES:
-- 0 = POSTED
-- 1 = MATCHED
-- 2 = COMPLETED
-- 3 = CANCELLED
--
-- URGENCY:
-- 0 = LOW, 1 = MEDIUM, 2 = HIGH
----------------------------------------------------------------------

INSERT INTO requests (
    student_email,
    tutor_email,
    subject,
    description,
    urgency,
    status,
    has_rated,
    is_accepted,
    days
) VALUES
-- Posted, not yet accepted
('alice@tru.ca', 'sarah@edu.ca', 'Math',
 'Need help with calculus',
 2, 0, 0, 0, '1000000'),

('brian@tru.ca', 'jacob@edu.ca', 'Biology',
 'Lab report assistance',
 1, 0, 0, 0, '0110000'),

-- Matched, accepted
('cathy@tru.ca', 'olivia@edu.ca', 'English',
 'Essay proofreading',
 0, 1, 0, 1, '0011000'),

('david@tru.ca', 'mike@edu.ca', 'Computer Science',
 'Struggling with pointers and memory',
 2, 1, 0, 1, '1110000'),

-- Completed + Rated
('emily@tru.ca', 'sophie@edu.ca', 'Economics',
 'Microeconomics midterm review session',
 1, 2, 1, 1, '0101000'),

('frank@tru.ca', 'sarah@edu.ca', 'Physics',
 'Final exam preparation help',
 2, 2, 1, 1, '1010100'),

-- Cancelled
('alice@tru.ca', 'olivia@edu.ca', 'History',
 'Had to cancel due to conflict',
 0, 3, 0, 0, '1000100');
