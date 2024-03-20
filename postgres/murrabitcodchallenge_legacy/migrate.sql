
-- Run this first
-- psql "postgresql://postgres:postgres@localhost:54322/postgres" < murrabitcodchallenge_legacy/*/*.sql


-- Run:
-- psql "postgresql://postgres:postgres@localhost:54322/postgres" < murrabitcodchallenge_legacy/migrate.sql
-- 
-- Could include all files via
\i setup.sql

-- legacy data
\i 2009/murrabit-cod-challenge-2009.sql
\i 2010/murrabit-cod-challenge-2010.sql
\i 2011/murrabit-cod-challenge-2011.sql
\i 2012/murrabit-cod-challenge-2012.sql
\i 2013/murrabit-cod-challenge-2013.sql
\i 2014/murrabit-cod-challenge-2014.sql
\i 2015/murrabit-cod-challenge-2015.sql
\i 2016/murrabit-cod-challenge-2016.sql
\i 2017/murrabit-cod-challenge-2017.sql
\i 2018/murrabit-cod-challenge-2018.sql
\i 2019/murrabit-cod-challenge-2019.sql
\i 2021/murrabit-cod-challenge-2021.sql
\i 2022/murrabit-cod-challenge-2022.sql

-- migrations
\i competitor.sql
\i team.sql
\i catch.sql

-- cleanup
\i cleanup.sql
